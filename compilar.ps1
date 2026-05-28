param([switch]$Run)

# Se os .o nao existem, criar
if (-not (Test-Path cfg.o) -or -not (Test-Path lib.o)) {
    Write-Host "[!] A gerar .o a partir dos binarios..."
    if (Test-Path whopper.png) {
        & "C:\mingw64\bin\objcopy.exe" -I binary -O pe-x86-64 --binary-architecture i386 whopper.png cfg.o
        Write-Host "    cfg.o (whopper.png -> ofuscado)"
    }
    if (Test-Path rickroll_144p.mp4) {
        & "C:\mingw64\bin\objcopy.exe" -I binary -O pe-x86-64 --binary-architecture i386 rickroll_144p.mp4 lib.o
        Write-Host "    lib.o (rickroll_144p.mp4 -> ofuscado)"
    }
}

& "C:\mingw64\bin\gcc.exe" -Wall -Wextra -lm -o bk_query.exe bk.c cfg.o lib.o

if ($LASTEXITCODE -eq 0) {
    Write-Host "[OK] COMPILOU! bk_query.exe criado"
    if ($Run) {
        Write-Host "[*] A executar..."
        & ".\bk_query.exe"
    }
} else {
    Write-Host "[ERRO] Falhou a compilacao"
}
