#!/usr/bin/env bash

project_name='prog3_adaptors_v2026_2'
source_code='
  include/merge_stacks.h
  include/join_stacks.h
  include/process_text_by_stack.h
  include/process_text_by_queue.h
  include/is_equation_balanced.h
  include/browser_history.h
  include/validate_tags.h
  include/evaluate_postfix.h
  '
cd .. || exit 1
rm -f ${project_name}.zip
zip -r -S ${project_name} ${source_code}
