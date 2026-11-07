# Contexto del proyecto Lumiere

> Estado relevado el 2026-11-07 mediante inspección estática. No se compiló,
> simuló, flasheó ni accionó hardware durante esta revisión.

## Identidad

- **Objetivo inferido:** firmware de control de un equipo Lumiere basado en
  iluminación de potencia. Permite seleccionar color/función, potencia y
  tiempo de trabajo; controla una interfaz gráfica táctil, sonido, temperatura,
  ventilador, reloj y contadores de uso.
- **Plataforma:** microcontrolador Freescale/NXP MC9S08JM60 (HCS08, modelo de
  memoria SMALL), con proyecto de CodeWarrior/Processor Expert.
- **Alcance actual observado:** firmware, recursos gráficos y sonoros,
  configuración del MCU, scripts de depuración BDM y artefactos de una
  compilación histórica.
- **Fuera de alcance de esta revisión:** validación clínica/eléctrica,
  comportamiento real de sensores y actuadores, compatibilidad exacta del IDE,
  flasheo y pruebas sobre el equipo.
- **Estado:** proyecto histórico recuperado, con binario generado, pero sin
  evidencia reproducible ni validación actual. Antes de evolucionarlo hay que
  confirmar cuál de las dos variantes incluidas es la vigente.
- **Próximo resultado observable recomendado:** abrir y compilar la variante
  raíz sin modificarla, registrar advertencias y comprobar en simulador las
  transiciones básicas de UI, temporización, temperatura y persistencia.

## Fuente canónica provisional

Se toma **provisionalmente** como variante principal:

- proyecto: `Lumiere.mcp`;
- código: `Sources/`;
- linker: `prm/Project.prm`;
- artefactos: `bin/`.

La razón es que `bin/Project.map` enumera los objetos de esa variante, incluidos
`RtcUtil.c.o`, `Lumiere.c.o` y `DisplayTools.c.o`.

Existe una segunda copia completa en `Lumiere/Lumiere/`. No es un respaldo
idéntico: varios archivos difieren, la copia raíz contiene `RtcUtil.*` y
`DisplayToolsCav.*`, mientras la anidada contiene `Keyboard.*`. No mezclar ni
eliminar ninguna hasta identificar su procedencia y finalidad.

## Entorno y compilación

- Abrir `Lumiere.mcp` con una versión de CodeWarrior for Microcontrollers que
  soporte HCS08/MC9S08JM60 y Processor Expert.
- Target de depuración observado: HCS08 Open Source BDM.
- Linker registrado en el mapa histórico: SmartLinker V-5.0.37 Build 9279.
- Salidas esperadas: `bin/Project.abs`, `bin/Project.abs.s19` y
  `bin/Project.map`.
- No se encontró compilación por línea de comandos, suite de pruebas ni CI.
- El proyecto contiene archivos generados por Processor Expert (`MCUinit.*` y
  configuración asociada); confirmar el flujo de regeneración antes de
  editarlos manualmente.

### Validación propuesta

1. Abrir el proyecto raíz en el IDE sin convertirlo todavía.
2. Ejecutar `Clean` y `Make` del target estándar.
3. Comparar el mapa nuevo con `bin/Project.map`: secciones, símbolos, RAM,
   tamaño descargable y vectores.
4. Ejecutar primero en el simulador HCS08 si el entorno lo permite.
5. Para pruebas físicas, usar un equipo de laboratorio y autorización explícita;
   verificar salidas con instrumentación antes de conectar cargas reales.

## Arquitectura observada

El diseño es procedural y está fuertemente acoplado al hardware:

1. `main.c` llama a `MCU_init()` y luego a `Main()`.
2. `Main()` restaura configuración de flash, inicializa el display y entra en
   `NavMenu()`.
3. `NavMenu()` implementa una máquina de navegación por pantallas: bienvenida,
   reloj, menú, selección, ajustes, control, trabajo, configuración, clave y
   calibración táctil.
4. `Lumiere.c` concentra lógica de UI, sesión, potencia, temperatura,
   persistencia y parte del trabajo de interrupciones.
5. `Display*`, `TouchTools`/`TchScr`, `SoundsTools`, `Dallas`, `RtcUtil` y
   `FlashTool` encapsulan parcialmente periféricos concretos.
6. `ScreenData.*` y `SoundData.*` contienen recursos embebidos.

### Hardware identificado

- PWM RGB y backlight/contraste;
- display gráfico y panel táctil;
- sensor de temperatura Dallas/1-Wire;
- ventilador de refrigeración;
- RTC por interrupción;
- almacenamiento flash interno;
- sonido/buzzer;
- depuración y programación mediante BDM.

## Memoria y artefacto existente

- Mapa de memoria: Z_RAM `0x00B0-0x00FF`, RAM `0x0100-0x10AF`, RAM1
  `0x1860-0x195F`, ROM principal `0x1960-0xFFAD` y stack configurado en
  `0x80` bytes.
- El mapa histórico informa `.data` 95 B, `.bss` 36 B, `MY_ZEROPAGE` 47 B,
  `.rodata` 10.981 B y `.text` 13.706 B.
- Tamaño histórico descargable: 31.196 bytes.
- Último artefacto encontrado: `bin/Project.abs.s19`, fechado 2021-10-28.
- SHA-256 del S19: `08967EC2971B5511D09D46618762F1C621403282F1E5FF0266A5A0F9F01B58BD`.
- El artefacto sirve como referencia de recuperación, pero **no está marcado
  como estable** porque no hay evidencia de pruebas o correspondencia con una
  entrega aprobada.

## Persistencia

- `FlashData.asm` reserva datos de configuración desde `0xE000`.
- Se guardan ajuste de reloj, brillo, contraste, volumen, calibración táctil,
  contadores de uso, fecha/hora y porcentajes de mezcla de color.
- `FlashSaveData()` borra y reescribe el bloque; también se invoca durante el
  conteo de minutos de operación. Revisar endurance de flash y atomicidad ante
  pérdida de alimentación antes de mantener este esquema.
- `FlashRestoreData()` carga valores directamente. No se observó firma, versión,
  CRC ni validación integral del bloque persistido.
- Antes de cambiar el formato se necesita respaldo, migración compatible y un
  procedimiento de restauración separado del rollback del firmware.

## Seguridad, exposición y operación

- **Exposición observada:** equipo aislado, sin red ni servicios remotos en el
  código revisado.
- **Perfil recomendado:** reforzado por controlar hardware de potencia y por su
  posible uso médico/estético, aunque no tenga exposición de red.
- No se observaron datos personales; sí configuración, calibración, reloj y
  contadores de uso.
- Existe una clave numérica de mantenimiento codificada directamente en
  `Lumiere.c`; debe considerarse una barrera de interfaz, no autenticación
  segura. Su valor no se replica en esta documentación.
- El watchdog COP aparece deshabilitado (`COPT=0` en `MCUinit.c`). Confirmar si
  existe una justificación de seguridad y definir estado seguro ante bloqueo.
- No se encontró logging persistente, registro de fallas, métricas ni códigos de
  diagnóstico recuperables. La observabilidad parece limitarse a la pantalla.
- No flashear ni accionar salidas sin autorización explícita y un protocolo de
  banco que defina estado seguro, límites y parada de emergencia.

## Riesgos y deuda técnica priorizada

1. **Variante vigente no identificada:** hay dos árboles divergentes y no hay
   control de versiones en esta carpeta.
2. **Estado no reproducido:** el binario de 2021 no fue recompilado ni probado.
3. **Sin pruebas automatizadas:** temporización, estados, límites térmicos,
   persistencia y fallas de sensor dependen de prueba manual o hardware.
4. **Responsabilidades concentradas:** `Sources/Lumiere.c` tiene 1.942 líneas y
   mezcla UI, reglas de operación, hardware y persistencia. También superan 500
   líneas `DisplayTools.c` y varios archivos de recursos/assembler; distinguir
   archivos generados de código mantenido antes de refactorizar.
5. **Persistencia frágil:** escrituras frecuentes, sin versión/CRC y sin
   recuperación transaccional visible.
6. **Recuperación incompleta:** no hay Git, tags, changelog ni relación trazable
   entre el S19, el hardware compatible y una prueba aprobada.
7. **Estado seguro por verificar:** watchdog deshabilitado y manejo de fallas de
   sensor/temperatura no caracterizado con pruebas.

## Git y recuperación

- Esta carpeta no contiene repositorio Git ni remoto conocido.
- No crear ni vincular un remoto, publicar o hacer push sin autorización.
- Antes de modificar código, conservar una copia verificable de ambas variantes,
  del `.mcp`, de la configuración de Processor Expert y del S19 existente.
- Si se autoriza Git, usar `main` como rama permanente, excluir objetos
  regenerables y documentar explícitamente qué artefactos binarios deben
  conservarse por recuperación de herramientas antiguas.

## Decisiones pendientes

- Confirmar si la variante raíz o `Lumiere/Lumiere/` es la que corresponde al
  equipo actual.
- Confirmar modelo comercial, revisión de PCB, periféricos y función clínica o
  estética exacta.
- Identificar versión instalable del IDE, compilador y licencia.
- Definir límites seguros de potencia, temperatura, tiempo y respuesta a sensor
  ausente o inválido.
- Decidir si el binario histórico debe preservarse como entrega formal.
- Decidir si se inicializa Git y dónde estará el remoto autorizado.

## Última revisión significativa

- **2026-11-07:** inventario estático y creación de este contexto. Se identificó
  la fuente canónica provisional, la duplicación divergente, el entorno de
  compilación, la arquitectura, la persistencia y los riesgos principales. No
  hubo cambios de firmware ni validación sobre hardware.
