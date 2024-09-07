directories=$(ls -d ./tests/* | grep -E '[0-9]+-.*' | sort -V)
PASSED=true
total=$(echo $directories | wc -w)

echo -e "\e[33mCompiling...\e[0m"
make >/dev/null 2>&1

echo -e "\e[33mRunning $total tests\e[0m"
for dir in $directories; do
    number=$(echo "$dir" | sed -E 's/.*\/([0-9]+)-.*/\1/')
    echo -e "\e[33mRunning test $number\e[0m"

    make run <"$dir/test.in" >"$dir/test.tmp"

    if [ -f "$dir/err" ]; then
        rm "$dir/err"
    fi

    if ! diff -wq "$dir/test.out" "$dir/test.tmp" >/dev/null; then
        echo -e "\e[31mTest $dir FAILED\e[0m"
        diff "$dir/test.out" "$dir/test.tmp" >"$dir/err"
        PASSED=false
    fi

    rm "$dir/test.tmp"
done

if $PASSED; then
    echo -e "\e[32mAll tests PASSED\e[0m"
else
    echo -e "\e[31mSome tests FAILED. Check the 'err' files.\e[0m"
fi
