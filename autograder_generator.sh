#!/usr/bin/env bash
set -euo pipefail

project_name="prog3_adaptors_v2026_2"
source_code=(
    "include/merge_stacks.h"
    "include/join_stacks.h"
    "include/process_text_by_stack.h"
    "src/process_text_by_stack.cpp"
    "include/process_text_by_queue.h"
    "src/process_text_by_queue.cpp"
    "include/is_equation_balanced.h"
    "src/is_equation_balanced.cpp"
    "include/browser_history.h"
    "src/browser_history.cpp"
    "include/validate_tags.h"
    "src/validate_tags.cpp"
    "include/evaluate_postfix.h"
    "src/evaluate_postfix.cpp"
    "include/stack_utec.h"
    "include/queue_utec.h"
    "include/adaptor_concepts.h"
)
project_root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
output_dir="$(cd "${project_root}/.." && pwd)"
autograder_zip="${output_dir}/autograder.zip"
submission_zip="${output_dir}/${project_name}.zip"

# Create autograder zip file
rm -f "${autograder_zip}"

cd "${project_root}/autograder/tests" || exit 1
rm -f "${source_code[@]}"
cd .. || exit 1
zip -r "${autograder_zip}" .
cd ../tools/source/ || exit 1
zip -ur "${autograder_zip}" .
cd "${project_root}" || exit 1

# Create the student submission zip file.
rm -f "${submission_zip}"
zip -r "${submission_zip}" "${source_code[@]}"

echo "Autograder generado: ${autograder_zip}"
echo "Entrega de prueba generada: ${submission_zip}"
