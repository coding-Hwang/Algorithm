#!/bin/bash

# 알고리즘 이름
ALGORITHMS=("merge" "heap" "bubble" "insertion" "selection" "quick")

# 입력 데이터 유형
INPUT_TYPES=("sorted" "random" "reverse" "partial")

# 반복 실행
for algo in "${ALGORITHMS[@]}"
do
    EXEC="./experiment_${algo}"
    
    if [ ! -f "$EXEC" ]; then
        echo "❌ $EXEC not found. Please compile it first."
        continue
    fi

    for input_type in "${INPUT_TYPES[@]}"
    do
        echo "▶ Running $algo sort for input: $input_type"
        $EXEC "$input_type"
    done
done

echo "✅ All convention algorithm experiments completed."
