pipeline {
    agent any

    stages {

        stage('Build Backend Image') {
            steps {
                sh """
                docker rmi -f backend-app || true
                docker build -t backend-app backend
                """
            }
        }

        stage('Ensure Docker Network Exists') {
            steps {
                sh """
                docker network inspect lab6-network >/dev/null 2>&1 || \
                docker network create lab6-network
                """
            }
        }

        stage('Deploy Backend Containers') {
            steps {
                sh """
                docker rm -f backend1 backend2 || true

                docker run -d --name backend1 --network lab6-network backend-app
                docker run -d --name backend2 --network lab6-network backend-app

                sleep 3
                """
            }
        }

        stage('Deploy NGINX Load Balancer') {
            steps {
                sh """
                docker rm -f nginx-lb || true

                docker run -d --name nginx-lb --network lab6-network -p 80:80 nginx

                sleep 3

                docker cp nginx/default.conf nginx-lb:/etc/nginx/conf.d/default.conf

                docker exec nginx-lb nginx -s reload
                """
            }
        }
    }

    post {
        success {
            echo 'Pipeline executed successfully!'
        }
        failure {
            echo 'Pipeline failed. Check console logs for errors.'
        }
    }
}
