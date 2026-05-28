param(
    [Parameter(Mandatory=$true)]
    [string]$InputFile,
    [Parameter(Mandatory=$true)]
    [string]$OutputC,
    [Parameter(Mandatory=$true)]
    [string]$VarName
)

$bytes = [System.IO.File]::ReadAllBytes((Resolve-Path $InputFile))

$header = @"
/* Ficheiro gerado automaticamente por converter.ps1 */
/* NÃO EDITAR MANUALMENTE */
#include <stdint.h>
#include <stddef.h>

const uint8_t ${VarName}_data[] = {
"@

$hex = ""
for ($i = 0; $i -lt $bytes.Count; $i++) {
    if ($i % 16 -eq 0) {
        if ($i -gt 0) { $hex += "`n" }
        $hex += "    "
    }
    $hex += "0x{0:x2}, " -f $bytes[$i]
}
$hex += "`n"

$footer = @"
};
const size_t ${VarName}_size = sizeof(${VarName}_data);
"@

$content = "$header`n$hex$footer"
Set-Content -Path $OutputC -Value $content -Encoding ASCII

Write-Host ("[OK] " + $InputFile + " -> " + $OutputC + " (" + $bytes.Count + " bytes, var: " + $VarName + "_data)")
