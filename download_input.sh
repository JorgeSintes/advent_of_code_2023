day=$1

if [ -z "$day" ]; then
    echo "Usage: $0 <day>"
    exit 1
fi

#load dotenv, containing the AOC_SESSION_COOKIE
if [ -f .env ]; then
    export $(cat .env | xargs)
fi

if [ ! -d "$day" ]; then
    mkdir -p "day$day"
fi

curl https://adventofcode.com/2023/day/$day/input --cookie session=$AOC_SESSION_COOKIE > day$day/input.txt
head -n 5 day$day/input.txt > day$day/short_input.txt