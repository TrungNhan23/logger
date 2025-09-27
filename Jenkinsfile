pipeline {
    agent any

    tools {
        // Nếu Jenkins đã cài đặt trước CMake/GCC thì có thể gọi trực tiếp
    }

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
                    sudo apt-get install -y cmake g++ make
                '''
            }
        }

        stage('Build') {
            steps {
                sh '''
                    mkdir -p build
                    cd build
                    cmake ..
                    make -j$(nproc)
                '''
            }
        }

        stage('Run tests') {
            steps {
                sh '''
                    cd build
                    ctest --output-on-failure
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
