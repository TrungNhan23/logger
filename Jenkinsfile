pipeline {
    agent any
    
    stages {
        stage('Checkout') {
            steps {
                // Clone repo
                checkout scm
            }
        }

        stage('Install dependencies') {
            steps {
                sh '''
                    sudo apt-get update
                    sudo apt-get update && sudo apt-get install -y build-essential cmake libgtest-dev
                '''
            }
        }

        stage('Build') {
            steps {
                sh '''
                    cmake --build build -- -j$(nproc)
                '''
            }
        }

        stage('Run tests') {
            steps {
                sh '''
                    cd build
                    for testfile in build/tests/*; do
                        if [ -f "$testfile" ] && [ -x "$testfile" ]; then
                            echo "Running $testfile"
                            "$testfile" || exit 1
                    fi
                    done
                '''
            }
        }
    }

    post {
        always {
            junit 'build/**/*.xml'  // nếu test sinh ra report junit
            archiveArtifacts artifacts: 'build/**', fingerprint: true
        }
    }
}
