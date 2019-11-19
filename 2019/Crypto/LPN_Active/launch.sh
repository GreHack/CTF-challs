while true; do
    socat -T5 tcp-listen:4361,fork,reuseaddr exec:'python tag.py'
done
