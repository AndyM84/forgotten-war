<#

.SYNOPSIS
Retrieve a set of information about the current system.

.DESCRIPTION
Retrieves and displays several pieces of information about the current system.

.EXAMPLE
Show information about the current system.

bin/Get-SystemInfo.ps1

#>
Write-Host "-> Gathering system information"
Write-Host ""
Write-Host "Computer Name: $($env:computername)"
Get-WmiObject -Class Win32_LogicalDisk

$uptime = (get-date) - (gcim Win32_OperatingSystem).LastBootUpTime
Write-Host "Host uptime: $($uptime)"

$os = Get-Ciminstance Win32_OperatingSystem
$os | Select-Object @{Name = "MEM FreeGB";Expression = {[math]::Round($_.FreePhysicalMemory/1mb,2)}},
	@{Name = "MEM TotalGB";Expression = {[int]($_.TotalVisibleMemorySize/1mb)}}

Write-Host ".NET Core v" -NoNewline
Invoke-Expression "dotnet --version"

Write-Host "PS Version: $($PSVersionTable.PSVersion)"

Write-Host ""
Write-Host "-> Done gathering system information"