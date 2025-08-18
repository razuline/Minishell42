 Concepts de haut niveau
✅ 1. Programmation système Unix/Linux

    Utilisation de fork, execve, pipe, dup2, wait, waitpid, signal, etc.

    Gestion de processus enfants.

    Manipulation des file descriptors.

✅ 2. Gestion de la mémoire

    Allocation/désallocation dynamique avec malloc, free.

    Eviter les fuites mémoire : nettoyage propre à chaque commande.

    Gestion des tableaux de chaînes (char **).

✅ 3. Parsing / Lexing

    Implémentation d’un lexer pour découper l'entrée utilisateur en tokens.

    Analyse syntaxique (parser) : organisation des tokens en commandes.

    Traitement des quotes, variables d’environnement, opérateurs (|, >, <).

✅ 4. Variables d’environnement

    Duplication (environ → envp local).

    Manipulation avec export, unset, env.

    Implémentation de getenv / setenv customisés.

✅ 5. Builtins

    Implémentation des commandes internes : cd, echo, pwd, export, unset, env, exit.

	Commandes :
	
	✳️​​ echo

	Rôle : Affiche du texte à l'écran.

	Fonctionnement : Ta version de builtin_echo gère le cas de base ainsi que l'option -n pour supprimer le saut de ligne final. Elle parcourt les arguments, les affiche séparés par un espace, et décide d'ajouter ou non un \n à la fin. C'est une implémentation simple et efficace.

	✳️​ cd

	Rôle : Change le répertoire de travail actuel.

	Fonctionnement : C'est une commande plus complexe que tu as bien gérée.

	cd sans argument t'emmène au répertoire HOME en allant chercher la variable d'environnement correspondante.

	cd [chemin] tente de se déplacer vers le chemin spécifié en utilisant l'appel système chdir.

	Mise à jour de l'environnement : Très important, après un cd réussi, ta fonction update_pwd_vars met à jour les variables PWD (nouveau répertoire) et OLDPWD (ancien répertoire), ce qui est crucial pour la navigation.

	✳️​ pwd

	Rôle : Affiche le chemin du répertoire de travail actuel (Print Working Directory).

	Fonctionnement : C'est la plus simple des built-ins. Elle utilise l'appel système getcwd (get current working directory) pour obtenir le chemin et l'affiche à l'écran.

	✳️​ export
	
	Rôle : Crée ou modifie des variables d'environnement.

	Fonctionnement : C'est l'une des built-ins les plus complexes.

	export sans argument : Tu as implémenté une fonctionnalité très propre. Le code trie par ordre alphabétique et affiche toutes les variables d'environnement avec le préfixe export et des guillemets autour de la valeur, comme le fait bash.

	export VAR=valeur : Ajoute ou modifie la variable VAR.

	export VAR : Crée une variable sans valeur, qui sera affichée par export mais pas par env.

	Validation : Tu vérifies bien que le nom de la variable est valide (is_valid_varname) avant de l'ajouter.

	✳️​ unset

	Rôle : Supprime une variable d'environnement.

	Fonctionnement : La fonction parcourt les arguments fournis, vérifie que chaque nom de variable est valide, puis appelle unset_env_var pour la retirer de l'environnement copié par ton shell. C'est le complément logique de export.

	✳️​ env

	Rôle : Affiche les variables d'environnement.

	Fonctionnement : Ta fonction builtin_env parcourt simplement le tableau state->envp et affiche chaque chaîne qui contient un =, c'est-à-dire les variables qui ont une valeur.

	✳️​ exit

	Rôle : Termine le processus du shell.

	Fonctionnement :

	exit sans argument quitte avec le code de sortie de la dernière commande exécutée.

	exit [n] quitte avec le code de sortie n (modulo 256).

    Ne pas les lancer avec execve, mais les exécuter directement dans le process parent.

✅ 6. Redirections / Pipes

    Gestion des redirections :

        >, >>, <, << (heredoc)

    Gestion des pipes (|) avec création de processus liés par des file descriptors.

✅ 7. Heredocs

    Implémentation d'un mini-shell dans le shell pour lire une entrée jusqu’à un délimiteur.

    Souvent sujet à gestion fine des signaux.

✅ 8. Gestion des signaux

    Réagir proprement à Ctrl+C, Ctrl+D, Ctrl+\.

    Modification des handlers avec signal ou sigaction.

✅ 9. Interaction avec readline

    Utilisation de la lib readline :

        readline(), add_history().

    Facilité d’usage, mais demande de bien gérer la libération mémoire de line.

✅ 10. Gestion des erreurs

    Messages d’erreurs comme bash : command not found, permission denied, etc.

    Codes de retour (exit_code).

✅ 11. Structures de données

    Listes chaînées pour les tokens, commandes, redirections.

    Construction d’arbres syntaxiques possible (optionnel/bonus).

🎯 Bonus éventuels

    Support des opérateurs logiques &&, ||.

    Wildcards (*) avec glob.

    Variables locales (VAR=val).

    Expansion de ~ et * (glob).

    Support de () pour des sous-shells.

📚 Compétences techniques consolidées

    Bonne connaissance de la norme POSIX.

    Gestion rigoureuse de la mémoire et des pointeurs.

    Travail en équipe (Makefile, gestion modulaire des fichiers).

    Habitude de lire et comprendre des man pages.

    Rigueur dans la norme 42.