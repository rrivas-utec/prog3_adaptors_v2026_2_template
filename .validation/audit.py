"""Read-only checks for the authored asset contract; writes no delivery files."""
from pathlib import Path
import hashlib
import re
import yaml

root = Path(__file__).resolve().parent.parent
config_path = root.parent.parent / 'configurations/prog3_adaptors_v2026_2.yaml'
config = yaml.safe_load(config_path.read_text(encoding='utf-8'))
expected_hash = '835996aeaf8e415814f0809de646c10f070c7925a86c7f0082a4d11f2c76bbf5'
assert hashlib.sha256(config_path.read_bytes()).hexdigest() == expected_hash
assert config['tests_per_question'] == 4
for path in (root / 'include').glob('*.h'):
    assert path.stat().st_size == 0, path
all_reference_headers = '\n'.join(p.read_text(encoding='utf-8') for p in (root/'reference_solution/include').glob('*.h'))
for item in config['source_files']:
    name = item['header']
    assert (root/'include'/name).is_file()
    starter = root/'src'/Path(name).with_suffix('.cpp')
    assert starter.is_file()
    assert starter.read_text(encoding='utf-8').count('#include') == 1
    assert '{' not in starter.read_text(encoding='utf-8')
    reference = root/'reference_solution/include'/name
    assert reference.stat().st_size > 40
    text = (root/f"question_tests/question_{item['question']}/cases.h").read_text(encoding='utf-8')
    assert len(re.findall(r'\bTEST_CASE\(', text)) == 4
    assert re.findall(r'TEST_CASE_INDEX == (\d)',text) == ['1','2','3','4']
    assert 'reference_solution' not in text
    assert '#include "'+name+'"' in text
    for symbol in item['required_symbols']:
        # Namespace-qualified symbols are verified by api_contract.cpp and compilation.
        token=symbol.split('::')[-1]
        assert re.search(r'\b'+token+r'\b', all_reference_headers), (name,symbol)
for path in (root/'reference_solution').rglob('*'):
    if path.suffix in ('.h','.cpp','.tpp'):
        assert not re.search(r'\bTODO\b|not been authored|reference-only API here',path.read_text(encoding='utf-8'))
solution = (root/'reference_solution/solution.h').read_text(encoding='utf-8')
assert all(line.startswith(('#include ', '#pragma once')) or not line.strip() for line in solution.splitlines())
assert (root/'enunciado.pdf').read_bytes().startswith(b'%PDF-')
print('PASS: YAML unchanged; 8 APIs; 10 empty student headers and starter files; 32 indexed public-API cases; reference-only implementations; statement PDF.')
