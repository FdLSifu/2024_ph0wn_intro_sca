#!/bin/sh

if [ "$1" = "aes" ]; then
  attack="attack_aes"
elif [ "$1" = "rsa" ]; then
  attack="attack_rsa"
else
  echo "Usage: $0 {aes|rsa}"
  exit 1
fi

if docker info > /dev/null 2>&1; then
  docker_cmd="docker"
else
  docker_cmd="sudo docker"
  cp $HOME/.Xauthority /tmp/
fi

cd $(dirname $0)
$docker_cmd-compose -f docker-compose.$1.yml up --build --force-recreate --remove-orphans -d
$docker_cmd-compose -f docker-compose.$1.yml exec $attack bash
$docker_cmd-compose -f docker-compose.$1.yml down -t 0 
rm /tmp/.Xauthority
