#!/bin/bash

# --- CONFIGURAZIONE RICHIESTE ---
# Formato: "METODO URL CONTENUTO"
# Se CONTENUTO è un file esistente, verrà inviato quello.
# Altrimenti verrà inviato il testo inserito tra virgolette.
REQUESTS=(
    "GET  http://localhost:9021/tea🍵"
	"GET http://localhost:9021/script/pokedex/output/pokemon.cgi?value=gardevoir"
	"GET http://localhost:9021/script/pokedex/output/pokemon.cgi?value=vaporeon"
	"POST http://localhost:9021/script/crypter/exec 'ciao come stai?'"
	"POST http://localhost:9021/script/weaksleep/exec '10'"
    "POST http://localhost:9021/ 'Ciao, come stai?'"
    "POST http://localhost:9021/upload www/var/audio/home.mp3"
)

CONCURRENT=10
TIME="20s"

# Funzione per gestire il body (file o testo)
run_siege() {
    local METHOD=$1
    local URL=$2
    local BODY_RAW=$3

	echo "METHOD " $1
	echo "URL " $2
	echo "BODY_RAW " $3
	echo "----------------"
    # Se il metodo non è POST o PUT, esegui siege semplice
    if [[ "$METHOD" != "POST" && "$METHOD" != "PUT" ]]; then
        siege -c$CONCURRENT -t$TIME "$URL"
		echo "termino " "$URL"
        return
    fi

    # Gestione POST/PUT
    if [ -f "$BODY_RAW" ]; then
        # È un file: passiamo il contenuto del file
        echo "📤 Invio FILE: $BODY_RAW a $URL"
        siege -c$CONCURRENT -t$TIME "$URL $METHOD <$BODY_RAW"
		echo "termino " "$URL"
    else
        # È testo: lo scriviamo in un file temporaneo per Siege
        local TEMP_FILE=".temp_body_$(date +%s%N).txt"
        echo -n "$BODY_RAW" > "$TEMP_FILE"
        echo "✉️  Invio TESTO: '$BODY_RAW' a $URL"
        siege -c$CONCURRENT -t$TIME "$URL $METHOD <$TEMP_FILE"
		echo "termino " "$URL"
        rm "$TEMP_FILE"
    fi
}
clear
echo "🔥 Avvio test paralleli su processi separati..."

for REQ in "${REQUESTS[@]}"; do
    # Usiamo 'eval' per gestire correttamente le stringhe con spazi (testo)
    eval run_siege $REQ &
done

wait
echo "✅ Test completati."