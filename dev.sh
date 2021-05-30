#!/bin/bash
CC="/usr/bin/clang++" BAZEL_CXXOPTS="-std=c++17" bazel build -c opt //db:gendb //:main_dev
bazel-bin/main_dev --docroot "./resources" --resources-dir /usr/local/share/Wt/resources --http-listen 0.0.0.0:8080 --db_path bazel-bin/db/recipes.db --undefok=docroot,http-listen,resources-dir,c -c ./resources/wt_config.xml 
