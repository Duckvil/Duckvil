#!/usr/bin/env bash

if [ "$TRAVIS_BRANCH" != "development" ]; then 
    exit 0;
fi

travis login --github-token $GITHUB_TOKEN

# git checkout master || exit
# git merge --no-ff "$TRAVIS_COMMIT" || exit

# git push origin master

git checkout development || exit
git merge master

travis logout --com