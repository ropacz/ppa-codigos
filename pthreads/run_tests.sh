#!/bin/bash

# Configurações
MATRIX_SIZES=(100 200 300 400 500 600 700 800 900 1000 1100 1200 1300 1400 1500)
THREAD_COUNTS=(1 2 4 8 16 32)

SEQ_DIR="matriz_sequencial"
PAR_DIR="matriz_paralela"

SEQ_EXEC="./matriz_sequencial"
PAR_EXEC="./matriz_paralela"

OUTPUT_FILE="resultado_testes.txt"

# Compilação
echo "Compilando códigos..."
(cd "$SEQ_DIR" && make clean && make)
(cd "$PAR_DIR" && make clean && make)

echo "Resultados dos testes:" > "$OUTPUT_FILE"

# Associativo para guardar tempos sequenciais
declare -A SEQ_TIMES

# Função para medir tempo real em segundos
# Função para medir tempo real em segundos
measure_time() {
  start=$(date +%s.%N)
  "$@" > /dev/null 2>&1
  end=$(date +%s.%N)
  runtime=$(echo "$end - $start" | bc -l)
  echo "$runtime"
}

# Testes sequenciais
echo "Executando testes SEQUENCIAIS..." | tee -a "$OUTPUT_FILE"
for size in "${MATRIX_SIZES[@]}"; do
  echo "Tamanho da matriz: ${size} x ${size}" | tee -a "$OUTPUT_FILE"
  
  tempo=$(measure_time "${SEQ_DIR}/${SEQ_EXEC}" "$size")
  
  printf "Tempo real: %.4f segundos\n" "$tempo" | tee -a "$OUTPUT_FILE"
  SEQ_TIMES[$size]=$tempo
done

# Testes paralelos
echo -e "\nExecutando testes PARALELOS..." | tee -a "$OUTPUT_FILE"
for size in "${MATRIX_SIZES[@]}"; do
  SEQ_TIME=${SEQ_TIMES[$size]}
  
  for threads in "${THREAD_COUNTS[@]}"; do
    echo "Tamanho da matriz: ${size} x ${size} com ${threads} threads" | tee -a "$OUTPUT_FILE"
    
    tempo=$(measure_time "${PAR_DIR}/${PAR_EXEC}" "$size" "$threads")
    
    printf "Tempo real: %.4f segundos\n" "$tempo" | tee -a "$OUTPUT_FILE"
    
    # Calcula Speedup e Eficiência
    SPEEDUP=$(echo "scale=4; $SEQ_TIME / $tempo" | bc -l)
    EFICIENCIA=$(echo "scale=4; $SPEEDUP / $threads" | bc -l)
    
    printf "Speedup: %.4f\n" "$SPEEDUP" | tee -a "$OUTPUT_FILE"
    printf "Eficiência: %.4f\n" "$EFICIENCIA" | tee -a "$OUTPUT_FILE"
    echo "" | tee -a "$OUTPUT_FILE"
  done
done

echo "Testes finalizados! Resultados salvos em $OUTPUT_FILE."
