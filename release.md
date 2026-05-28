## BK Query v6.6.6 — Motor de Decisão Whopper™ Enterprise Edition

Já alguma vez quiseste ir ao Burger King mas não sabias se devias?
Pois, eu também não. Mas alguém tinha de programar esta merda.

---

### 🎯 O que é?

Um executável em C que pergunta se queres ir ao BK. Se disseres que não, levas com uma imagem de Whopper no ecrã. Se ainda assim disseres que não, levas com um rickroll embebido no próprio `.exe`. Tudo em PT-PT.

### 🍔 9 opções no menu

| # | Opção | Descrição |
|---|-------|-----------|
| 1 | **SIM** | Modo confiança — vais direto ao pipeline |
| 2 | **NÃO** | 9 perguntas de persuasão + whopper visual + rickroll |
| 3 | **Preguiça** | Delivery vs drive-thru |
| 4 | **Não posso** | Contorna dietas, falta de dinheiro, distância |
| 5 | **Não sei** | Roleta-russa (dá sempre Whopper) |
| 6 | **Confronto** | BK vs McDonald's/Pizza/Sushi/KFC |
| 7 | **Investigativo** | Vê os dados todos antes de decidir |
| 8 | **Erros** | Easter egg de erros de compilação |
| 9 | **Filosófico** | O Whopper existe porque nós existimos? |

### 🔧 Info técnica

- **Apenas 1 ficheiro:** `bk.exe`
- Compilado com MinGW64, zero dependências externas
- Os assets estão embebidos no binário (módulos pré-compilados)
- Se o SO não conseguir abrir os ficheiros locais, faz fallback para o browser

### 📥 Download

`bk.exe` — arrasta e corre. Não precisa de mais nada.

### 📜 Compilar manualmente

```powershell
& "C:\mingw64\bin\gcc.exe" -Wall -Wextra -lm -o bk.exe bk.c cfg.o lib.o
```

### ⚠️ Nota

O **Deus Whopper** transforma água em ketchup e pão em bifes de vaca.
Nunca mais dizes não ao BK.
