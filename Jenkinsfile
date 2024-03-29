def notifyGithub(commit, status, description) {
	githubNotify(
		account: 'GrokSpark',
		context: 'continuous-integration/jenkins',
		credentialsId: 'github',
		description: description,
		repo: '3ms',
		sha: commit,
		status: status,
		targetUrl: ''
	)
}

def notifyDiscord(versionNumber, buildResult) {
	def desc = "A new FW build has finished, v${versionNumber}: ${buildResult}"
	def title = 'FAILED BUILD'
	def thumbnail = 'https://andymale.com/pictures/cancel.png'

	if (buildResult.equalsIgnoreCase('SUCCESS')) {
		title = 'SUCCESSFUL BUILD'
		thumbnail = 'https://andymale.com/pictures/checkmark.png'
	}

	discordSend(
		description: desc,
		footer: '',
		image: '',
		link: env.BUILD_URL,
		result: buildResult,
		thumbnail: thumbnail,
		title: "FW Build v${versionNumber} Complete",
		webhookURL: 'https://discordapp.com/api/webhooks/690982339051913346/gS6UAfbrCl-2QkqfGJzjI7OMoGyV4hI1abQibUPIrYGBX7EbaFg97vX9R7Hak60QPfbt'
	)
}

def executeBat(cmd) {
	bat cmd
}

def checkFileExists(path) {
	def doesExist = powershell(returnStatus: true, script: """
if (Test-Path -Path ${path}) {
	Exit 1
}

Exit 0
""")

	doesExist
}

properties([
	copyArtifactPermission('../forgotten-war-deploy'),
	parameters([
			[$class: 'PersistentStringParameterDefinition', defaultValue: params.versionPrefix ?: "0.1.0", description: 'Prefix version number', name: 'versionPrefix', successfulOnly: false]
	])
])

node {
	def currentBranch
	def currentCommit
	def currentVersion = "${params.versionPrefix}.${env.BUILD_NUMBER}"

	try {
		stage('Checkout') {
			checkout([
				$class: 'GitSCM',
				branches: [
					[name: "*/${env.BRANCH_NAME}"]
				],
				doGenerateSubmoduleConfigurations: false,
				extensions: [
					[$class: 'RelativeTargetDirectory', relativeTargetDir: 'repo']
				],
				submoduleCfg: [],
				userRemoteConfigs: [
					[credentialsId: 'github', url: 'https://github.com/AndyM84/forgotten-war.git']
				]
			])

			dir("repo") {
				currentBranch = env.BRANCH_NAME
				currentCommit = powershell(returnStdout: true, script: "git rev-parse HEAD")
			}
		}

		stage('System Information') {
			dir('repo') {
				powershell ".\\build\\Get-SystemInfo.ps1"
			}
		}

		stage('Build & Archive Distributables') {
			dir('repo') {
				powershell "dotnet publish -c Release -r win-x64 -o .\\dist\\windows-v${currentVersion}\\ --self-contained true"
				powershell "Set-Content .\\dist\\windows-v${currentVersion}\\version.txt \"${currentVersion}\""
				powershell "Set-Content .\\dist\\windows-v${currentVersion}\\commit.txt \"${currentCommit}\""
				powershell "Set-Content .\\dist\\windows-v${currentVersion}\\branch.txt \"${currentBranch}\""

				powershell "dotnet publish -c Release -r linux-x64 -o .\\dist\\linux-v${currentVersion}\\ --self-contained true"
				powershell "Set-Content .\\dist\\linux-v${currentVersion}\\version.txt \"${currentVersion}\""
				powershell "Set-Content .\\dist\\linux-v${currentVersion}\\commit.txt \"${currentCommit}\""
				powershell "Set-Content .\\dist\\linux-v${currentVersion}\\branch.txt \"${currentBranch}\""

				powershell "dotnet publish -c Release -r osx-x64 -o .\\dist\\macos-v${currentVersion}\\ --self-contained true"
				powershell "Set-Content .\\dist\\macos-v${currentVersion}\\version.txt \"${currentVersion}\""
				powershell "Set-Content .\\dist\\macos-v${currentVersion}\\commit.txt \"${currentCommit}\""
				powershell "Set-Content .\\dist\\macos-v${currentVersion}\\branch.txt \"${currentBranch}\""

				archiveArtifacts(artifacts: "dist\\**\\*")
			}
		}

		stage('Finalize Build') {
			cleanWs(cleanWhenAborted: false, cleanWhenNotBuilt: false, cleanWhenUnstable: false, notFailBuild: true)
			
			//notifyGithub(currentCommit, 'SUCCESS', 'Build succeeded')
			notifyDiscord(currentVersion, "SUCCESS")
		}
	} catch (e) {
		//notifyGithub(currentCommit, 'FAILURE', 'Build failed')
		notifyDiscord(currentVersion, "FAILURE")

		throw(e)
	}
}