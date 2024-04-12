#!/bin/bash
git diff --name-only HEAD $(git rev-parse --abbrev-ref HEAD) > changed_files.list