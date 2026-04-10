#!/bin/zsh
# sugar.sh
# 2026/04/10
# Haruta Kutsukawa

# sugarscapeモデルを実行し
# 取引価格の平均と標準偏差をプロットする

# ------ 実行時の設定 ------ 
N=1000              # 反復回数
SEED=$1             # シード値
S_STEP=0            # 表示する最小ステップ
E_STEP=1000         # 表示する最大ステップ
INF_LIFESPAN=1      # エージェントの寿命を有効化/無効化
# -------------------------- 

make cleanall
make

NOW=`date "+%Y%m%d_%H%M%S"`

DATA_DIR=./data
IMG_DIR=./img

OUTPUT_FNAME="${DATA_DIR}/${NOW}_sugarscape_output.dat"
PRICE_STATS_FNAME="${DATA_DIR}/${NOW}_price_stats.dat"

PRICE_EPS_FNAME="${IMG_DIR}/${NOW}_price.eps"
PRICE_PDF_FNAME="${PRICE_EPS_FNAME%.eps}.pdf"

SETTING_LOG_FNAME="${DATA_DIR}/${NOW}_setting_log.dat"

if [[ ! -d ${DATA_DIR} ]]
then
  mkdir "${DATA_DIR}"
fi

if [[ ! -d ${IMG_DIR} ]]
then
  mkdir "${IMG_DIR}"
fi

echo "run ./sugar"
touch "${OUTPUT_FNAME}"
if [[ INF_LIFESPAN -eq 1 ]]
then
  ./sugar --seed ${SEED} --N ${N} --inf-lifespan > ${OUTPUT_FNAME}
else
  ./sugar --seed ${SEED} --N ${N} > ${OUTPUT_FNAME}
fi

touch "${PRICE_STATS_FNAME}"

: <<EOF
# 取引価格平均，標準偏差の配列を作成
# 1~N [step]の価格がもれなく，順番に並んでいる想定
# 改行をスペースとし，配列になおす
price_ave_arr=(`cat "${OUTPUT_FNAME}" | awk '$1=="pave"{print $3}' | sed 's/\n/ /g'`)
price_std_arr=(`cat "${OUTPUT_FNAME}" | awk '$1=="pstd"{print $3}' | sed 's/\n/ /g'`)

# ./sugarにpriceを全て出力させたときに，スクリプトで標準偏差を求める場合
for ((step=1; step<=N; step++))
do
  # ave=`cat "${OUTPUT_FNAME}" | awk -v step=${step} 'BEGIN{ sum=0.0; n=0; } $1=="price"&&$2==step{ sum+=$3; n++; } END{ if (n==0) { print 0; } else { print sum/n; } }'`
  # std=`cat "${OUTPUT_FNAME}" | awk -v step=${step} -v ave=${ave} 'BEGIN{ sum=0.0; n=0; } $1=="price"&&$2==step{ sum+=($3-ave)**2; n++; } END{ print sqrt(sum/n); }'`

  cat "${OUTPUT_FNAME}" | awk -v step=${step} '$1=="price"&&$2==step{ print $3; }' | wc -l
  ave=${price_ave_arr[${step}-1]}
  std=`cat "${OUTPUT_FNAME}" | awk -v step=${step} -v ave=${ave} 'BEGIN{ sum=0.0;n=0;  } $1=="price"&&$2==step{ sum+=($3-ave)**2; n++; } END{ if (n==0) { print "n/a"; } else { print sum/n; } }'`
  echo "${step} ${ave} ${std}" >> "${PRICE_STATS_FNAME}"
done
EOF

# 結果から，平均と標準偏差を抽出
cat "${OUTPUT_FNAME}" | awk '
  BEGIN{ ave=""; std=""; ave_step=-1; std_step=-1; }
  $1=="pave"{ ave=$3; ave_step=$2; } $1=="pstd"{ std=$3; std_step=$2; }
  ave!=""&&std!=""&&ave_step==std_step{ print ave_step, ave , std; ave=""; std=""; }' > "${PRICE_STATS_FNAME}"

echo "${price_ave}\n${price_std}"

gnuplot <<EOF
set terminal postscript eps color enhanced \
             font "Helvetica" 18
set output "${PRICE_EPS_FNAME}"

set xlabel "step"
set ylabel "price [spice / sugar]"

set xr [${S_STEP}:${E_STEP}]

plot "${PRICE_STATS_FNAME}" using 1:2:3 w yerrorbars lw 3 lc 3 title "", \
     "${PRICE_STATS_FNAME}" using 1:2:3 w lines lw 3 lc 3 title "average trade price"
EOF

epstopdf "${PRICE_EPS_FNAME}"

touch "${SETTING_LOG_FNAME}"
echo "N\t\t= ${N}" >> "${SETTING_LOG_FNAME}"
echo "SEED\t\t= ${SEED}" >> "${SETTING_LOG_FNAME}"
echo "S_STEP\t\t= ${S_STEP}" >> "${SETTING_LOG_FNAME}"
echo "E_STEP\t\t= ${E_STEP}" >> "${SETTING_LOG_FNAME}"
echo "INF_LIFESPAN\t= ${INF_LIFESPAN}" >> "${SETTING_LOG_FNAME}"
echo >> "${SETTING_LOG_FNAME}"
echo "data: ${OUTPUT_FNAME}" >> "${SETTING_LOG_FNAME}"
echo " eps: ${PRICE_EPS_FNAME}" >> "${SETTING_LOG_FNAME}"
echo " pdf: ${PRICE_PDF_FNAME}" >> "${SETTING_LOG_FNAME}"

open ${PRICE_PDF_FNAME}
