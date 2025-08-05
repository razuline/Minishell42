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