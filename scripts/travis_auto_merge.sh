#!/usr/bin/env bash

if [ "$TRAVIS_BRANCH" != "development" ]; then
    exit 0;
fi

cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug -- -j $(nproc)
ctest -j $(nproc) --output-on-failure

if [ $? -eq 0 ]
then
    echo "1. Tests passed successful"

    git fetch origin master:master
    git checkout -b validation development
    git merge master
    git stash save temp

    cmake -DCMAKE_BUILD_TYPE=Debug ..
    cmake --build . --config Debug -- -j $(nproc)
    ctest -j $(nproc) --output-on-failure

    if [ $? -eq 0 ]
    then
        echo "2. Tests passed successful, we can merge to master"

        git checkout master
        git merge development
        git push origin master

        lcov --capture --directory . --output-file coverage.info
        bash <(curl -Ls https://coverage.codacy.com/get.sh) report -r coverage.info

        exit 0;
    else
        echo "2. Tests did not pass, push validation branch to github"

        git checkout validation
        git stash pop
        git push origin validation

        exit 1;
    fi

    exit 0;
else
    echo "1. Tests did not pass"

    exit 1;
fi