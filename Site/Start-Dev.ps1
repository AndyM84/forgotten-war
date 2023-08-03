Start-Job -Name ZsfNpmDev -ScriptBlock{npm run dev}
Start-Job -Name ZsfPhpDev -ScriptBlock{cd web && php -S localhost:8080}