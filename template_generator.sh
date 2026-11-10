#!/usr/bin/env bash

project_name=prog3_adaptors_v2026_2_template

# Create Template
mkdir ../${project_name} -p
mkdir ../${project_name}/include -p
mkdir ../${project_name}/src -p
rsync -au template/include/ ../${project_name}/include
rsync -au template/src/ ../${project_name}/src
rsync -au public_tests ../${project_name}/
mkdir ../${project_name}/tools -p
rsync -au tools/catch ../${project_name}/tools/
rsync -a -f"- */" -f"+ *" ./ ../${project_name}
cp student_CMakeLists.txt ../${project_name}/CMakeLists.txt
cp template/README.md ../${project_name}/README.md
rm -f ../${project_name}/autograder_generator.sh
rm -f ../${project_name}/template_generator.sh
rm -f ../${project_name}/student_CMakeLists.txt
rm -rf ../${project_name}/.github

echo 'template folder has been synchronized correctly'
