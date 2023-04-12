#! /bin/bash
# SCRIPT PARA CALCULAR PORCENTAJES DE MAPAS

salida="pruebas/ejecucion_$1.txt"

if [ -f "$salida" ]; then
    rm "$salida"
fi

echo -e "Explorando mapa 30x30"
echo -e "MAPA 30x30" >> $salida
echo -e "\tNivel\tPorcentaje\tSobre 85" >> $salida

for (( i=0; i<=3; i++ ))
do
    echo -e "\tNivel: $i"
    resultado=$(./practica1SG ./mapas/mapa30.map 1 $i 6 6 1)
    porcentaje=$(echo "$resultado" | grep -oP 'Porcentaje de mapa descubierto:\s*\K[\d.]+')
    porcentaje_formateado=$(printf "%-7s" $porcentaje)
    if (( $(echo "$porcentaje >= 85" | bc -l) )); then
        echo -e "\t$i\t\t$porcentaje_formateado\t\t100" >> $salida
    else
        echo -e "\t$i\t\t$porcentaje_formateado\t\t$porcentaje_formateado" >> $salida
    fi
done

echo -e "Explorando mapa 50x50"
echo -e "MAPA 50x50" >> $salida
echo -e "\tNivel\tPorcentaje\tSobre 85" >> $salida

for (( i=0; i<=3; i++ ))
do
    echo -e "\tNivel: $i"
    resultado=$(./practica1SG ./mapas/mapa50.map 1 $i 3 3 0)
    porcentaje=$(echo "$resultado" | grep -oP 'Porcentaje de mapa descubierto:\s*\K[\d.]+')
    porcentaje_formateado=$(printf "%-7s" $porcentaje)
    if (( $(echo "$porcentaje >= 85" | bc -l) )); then
        echo -e "\t$i\t\t$porcentaje_formateado\t\t100" >> $salida
    else
        echo -e "\t$i\t\t$porcentaje_formateado\t\t$porcentaje_formateado" >> $salida
    fi
done

echo -e "Explorando mapa 75x75"
echo -e "MAPA 75x75" >> $salida
echo -e "\tNivel\tPorcentaje\tSobre 85" >> $salida

for (( i=0; i<=3; i++ ))
do
    echo -e "\tNivel: $i"
    resultado=$(./practica1SG ./mapas/mapa75.map 1 $i 60 11 0)
    porcentaje=$(echo "$resultado" | grep -oP 'Porcentaje de mapa descubierto:\s*\K[\d.]+')
    porcentaje_formateado=$(printf "%-7s" $porcentaje)
    if (( $(echo "$porcentaje >= 85" | bc -l) )); then
        echo -e "\t$i\t\t$porcentaje_formateado\t\t100" >> $salida
    else
        echo -e "\t$i\t\t$porcentaje_formateado\t\t$porcentaje_formateado" >> $salida
    fi
done

echo -e "Explorando mapa 100x100"
echo -e "MAPA 100x100" >> $salida
echo -e "\tNivel\tPorcentaje\tSobre 85" >> $salida

for (( i=0; i<=3; i++ ))
do
    echo -e "\tNivel: $i"
    resultado=$(./practica1SG ./mapas/mapa100.map 0 $i 84 39 6)
    porcentaje=$(echo "$resultado" | grep -oP 'Porcentaje de mapa descubierto:\s*\K[\d.]+')
    porcentaje_formateado=$(printf "%-7s" $porcentaje)
    if (( $(echo "$porcentaje >= 85" | bc -l) )); then
        echo -e "\t$i\t\t$porcentaje_formateado\t\t100" >> $salida
    else
        echo -e "\t$i\t\t$porcentaje_formateado\t\t$porcentaje_formateado" >> $salida
    fi
done
