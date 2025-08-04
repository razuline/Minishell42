#!/bin/bash

MINISHELL=./minishell

# Crée un fichier contenant toutes les commandes à envoyer à minishell
cat <<EOF > input.txt
echo Hello World
pwd
export VAR42=42
echo \$VAR42
cd /tmp
pwd
ls | wc -l
echo test > testfile
cat testfile
exit
EOF


$MINISHELL < input.txt
