# Validacion docente - etapa 2

Fecha: 2026-11-02. Resultado: PASS. Requiere revision docente antes de etapa 3.

## Entradas y alcance

- Configuracion: `../../configurations/prog3_adaptors_v2026_2.yaml`, relativa
  al asset root. SHA256 conservado:
  `835996aeaf8e415814f0809de646c10f070c7925a86c7f0082a4d11f2c76bbf5`.
- No existia PDF al comenzar. Se encontro el enunciado completo en
  `../../typst/ejercicios_adaptadores.typ` y se compilo sin cambiarlo.
- Solo se modifico el workspace de assets. No se ejecuto
  `create_instructor_assets.py`, `create_project.py` ni un generador de template.
- No se genero proyecto de entrega. CMake se uso exclusivamente como harness
  docente temporal en `.validation/build/`.

## Archivos y contrato

- Diez headers estudiantiles de cero bytes y diez esqueletos `.cpp`.
- Diez headers de referencia completos: ocho preguntas y dos adaptadores.
- Seis `.cpp` y cuatro `.tpp` de implementacion, todos en
  `reference_solution/src/`. Los `.tpp` se incluyen desde sus headers para
  instanciacion generica; no hay cuerpos de implementacion en los headers.
- Agregador `reference_solution/solution.h`: solo pragma e includes individuales.
- Ocho `cases.h`, cada uno con cuatro TEST_CASE y cuatro ramas TEST_CASE_INDEX.
  Incluyen headers publicos por nombre, sin rutas de referencia.
- `api_contract.cpp` verifica firmas exactas de las ocho APIs y metodos de
  Browser/BrowserHistory; tambien comprueba que Memento no expone su estado.
- Los tests verifican copia/asignacion de Memento y que su constructor de
  estado no sea publico. La revision docente debe confirmar las restricciones
  de implementacion (adaptadores publicos y patron Memento), que las pruebas
  funcionales por si solas no pueden garantizar para futuras entregas.

## Cobertura

| Pregunta | 1.00x | 1.25x | 1.50x | 2.00x |
|---|---|---|---|---|
| 1-2 | Vacios, ejemplo, unitarios | Matriz de longitudes | Strings, repetidos, copia, orden | Longitudes irregulares y oraculo vectorial |
| 3-4 | Vacios, pops sin datos, ejemplo | Combinaciones push/pop | Orden intercalado, llamadas repetidas, API queue | Flujos aleatorios y oraculo deque |
| 5 | Vacio, separadores aislados | Anidamiento y cruces | Enumeracion de 16807 cadenas | Profundidad 10000 y corrupcion |
| 6 | Historial vacio y retorno | Snapshots, copia, restauracion | Ramas, repetidos, scroll, independencia | Modelo secuencial de 20000 operaciones |
| 7 | Texto, ejemplos, pendientes | Sintaxis estricta y ASCII | Orden, mayusculas, llamadas repetidas | Profundidad 5000 y 3000 hermanos |
| 8 | Literales, ejemplo, vacio | Tokens invalidos y operandos | Orden de resta/division y errores con operandos pendientes | Oraculo aritmetico y 10000 operandos |

Cada cuarto caso tambien lanza dos llamadas simultaneas con datos propios.
Usa `std::async(std::launch::async, ...)`, esperas de hasta diez segundos y
recoge resultados con `get()`. No hay aserciones en trabajadores, sleeps,
datos mutables compartidos, medidas de speedup ni exigencia de seguridad
de una misma instancia compartida. El limite global de CTest es 120 segundos.
Iteradores, particiones de trabajo y coordinacion de tareas pendientes no
forman parte de este enunciado secuencial; no se inventaron APIs para ellos.

## Comandos ejecutados y resultados

Desde el asset root, en PowerShell:

```powershell
typst compile ../../typst/ejercicios_adaptadores.typ enunciado.pdf
typst compile --format png --ppi 60 ../../typst/ejercicios_adaptadores.typ '.validation/pdf/page-{p}.png'
cmake -S .validation -B .validation/build -G 'Visual Studio 17 2022' -A x64
cmake --build .validation/build --config Release --parallel 2
ctest --test-dir .validation/build -C Release --output-on-failure
./.validation/build/Release/validate_assets.exe --reporter compact
python .validation/audit.py
```

- MSVC 19.44.35213.0, C++20, x64, Release; compilacion final correcta.
- Se compilaron las 32 combinaciones pregunta/indice en unidades separadas,
  los seis `.cpp` de referencia y las comprobaciones estaticas de API.
- Catch2 existente en el repositorio: 32 casos, 77227 aserciones, todos pasan.
- CTest: 1/1 suite correcta, 0 fallos; 0.67 segundos en esta maquina.
  Este tiempo solo informa la ejecucion, no es un requisito de evaluacion.
- Auditoria: PASS para YAML, archivos, simbolos, ramas y aislamiento.
- PDF: diez paginas renderizadas y revisadas visualmente, sin recortes,
  superposiciones ni errores de fuentes visibles.

Se eliminaron build/, renders y scripts temporales de autoria. Se conservan
este reporte, CMakeLists.txt, api_contract.cpp y audit.py para repetir la
validacion. No quedan bloqueos tecnicos; queda la revision docente del
enunciado, referencia y cobertura antes de etapa 3.
