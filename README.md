# BK Query — Motor de Decisão Whopper™ Enterprise Edition

**Já alguma vez quiseste ir ao Burger King mas não sabias se devias?**
Pois, eu também não. Mas alguém tinha de programar esta merda.

## O que é isto?

Um programa em C que pergunta se queres ir ao BK. E depois faz-te 9 perguntas para ter a certeza. E se disseres que não, abre uma imagem de um Whopper no teu ecrã. E se ainda assim disseres que não, levas uma **surpresa**.

Tudo em PT-PT, como o Deus Whopper mandou (ele que transforma água em ketchup e pão em bifes de vaca).

## Como compilar

```powershell
.\compilar.ps1 -Run
```

Ou manualmente:

```powershell
& "C:\mingw64\bin\gcc.exe" -Wall -Wextra -lm -o bk.exe bk.c cfg.o lib.o; if ($?) { & ".\bk.exe" }
```

## Pré-requisitos

- MinGW64 (gcc)
- Opcional: ficheiros de asset se quiseres regenerar os `.o`

## Como funciona

O `bk.exe` já vem com todos os módulos ligados. Só precisas dele:

1. Corres o `.exe`
2. Escolhes uma das 9 opções do menu
3. O programa decide por ti (spoiler: vais ao BK)

### Opções disponíveis

| # | Opção | O que faz |
|---|-------|-----------|
| 1 | SIM | Pipeline direto |
| 2 | NÃO | 9 perguntas de persuasão + imagem do Whopper + rickroll |
| 3 | Com preguiça | Modo delivery vs drive-thru |
| 4 | Não posso | Contorna obstáculos (dieta/dinheiro/distância) |
| 5 | Não sei | Roleta-russa (dá sempre Whopper) |
| 6 | Confronto | BK vs McDonald's/Pizza/Sushi/KFC |
| 7 | Investigativo | Vê os dados todos antes de decidir |
| 8 | Erros | Easter egg de erros de compilação |
| 9 | Filosófico | Questões existenciais do Whopper |

## Estrutura de ficheiros

- `bk.c` — código fonte (847+ linhas de pura filosofia)
- `cfg.o` — módulo pré-compilado (não mexer)
- `lib.o` — módulo pré-compilado (não mexer)
- `compilar.ps1` — script de build
- `bk.exe` — executável final (só precisas deste)

## Fallback

Se não conseguir abrir os ficheiros embebidos, abre no browser.

## Licença

WTFPL — Do What The Fork You Want Public License

## Créditos

Programado por alguém com demasiado tempo livre e um vício em Whoppers.
