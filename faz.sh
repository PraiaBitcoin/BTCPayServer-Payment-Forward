#! /bin/bash

echo Starting...
for f in ./commands/*.csv 
do
  
  if [ ! -f "$f" ] 
  then
    echo "Done."
    break
  fi
  
  echo "Processing $f."

  cat "$f" | ./csv2invoice -a "$AMOUNT" -c "$COMMENT"
  mv "$f" "$f.done"
  sleep 1
  
done

exit 0
