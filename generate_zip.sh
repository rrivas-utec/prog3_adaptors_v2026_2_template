#!/usr/bin/env bash
set -euo pipefail

project_name='prog3_adaptors_v2026_2'
source_code=(
  include/merge_stacks.h
  include/join_stacks.h
  include/process_text_by_stack.h
  include/process_text_by_queue.h
  include/is_equation_balanced.h
  include/browser_history.h
  include/validate_tags.h
  include/evaluate_postfix.h
)
project_root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
output_path="${project_root}/${project_name}.zip"

cd "${project_root}"
rm -f "${output_path}"
zip -r -S "${output_path}" "${source_code[@]}"
echo "Archivo generado: ${output_path}"
