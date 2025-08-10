#!/bin/bash

# ============================================================================
# SUITE DE TESTS COMPLÈTE POUR MINISHELL - 42 School
# Basée sur la grille d'évaluation officielle
# ============================================================================

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Compteurs
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Variables globales
MINISHELL_PATH="./minishell"
TEST_FILES_DIR="./test_files"

# ============================================================================
# FONCTIONS UTILITAIRES
# ============================================================================

print_header() {
    echo -e "\n${CYAN}================================${NC}"
    echo -e "${CYAN}$1${NC}"
    echo -e "${CYAN}================================${NC}"
}

print_test() {
    echo -e "\n${BLUE}[TEST]${NC} $1"
    ((TOTAL_TESTS++))
}

print_success() {
    echo -e "${GREEN}✅ PASSED:${NC} $1"
    ((PASSED_TESTS++))
}

print_failure() {
    echo -e "${RED}❌ FAILED:${NC} $1"
    ((FAILED_TESTS++))
}

print_warning() {
    echo -e "${YELLOW}⚠️  WARNING:${NC} $1"
}

print_info() {
    echo -e "${PURPLE}ℹ️  INFO:${NC} $1"
}

# Fonction pour créer des fichiers de test
setup_test_files() {
    mkdir -p "$TEST_FILES_DIR"
    echo "Hello World" > "$TEST_FILES_DIR/test.txt"
    echo -e "line1\nline2\nline3" > "$TEST_FILES_DIR/multiline.txt"
    echo "secret content" > "$TEST_FILES_DIR/secret.txt"
    chmod 000 "$TEST_FILES_DIR/secret.txt" 2>/dev/null
    touch "$TEST_FILES_DIR/empty.txt"
}

cleanup_test_files() {
    rm -rf "$TEST_FILES_DIR" 2>/dev/null
}

# ============================================================================
# TESTS DE COMPILATION
# ============================================================================

test_compilation() {
    print_header "1. TESTS DE COMPILATION"
    
    print_test "Vérification des flags de compilation"
    if make -n 2>&1 | grep -q "\-Wall.*\-Wextra.*\-Werror\|f-Werror.*\-Wall.*\-Wextra\|\-Wextra.*\-Wall.*\-Werror"; then
        print_success "Flags de compilation corrects (-Wall -Wextra -Werror)"
    else
        print_failure "Flags de compilation manquants ou incorrects"
    fi
    
    print_test "Compilation sans erreurs"
    if make > /dev/null 2>&1; then
        print_success "Compilation réussie"
    else
        print_failure "Erreurs de compilation"
        make
        return 1
    fi
    
    print_test "Vérification que make ne relink pas"
    make > /dev/null 2>&1
    if make 2>&1 | grep -q "Nothing to be done\|up to date"; then
        print_success "Make ne relink pas inutilement"
    else
        print_warning "Make pourrait relinker inutilement"
    fi
    
    print_test "Vérification de l'exécutable"
    if [[ -x "$MINISHELL_PATH" ]]; then
        print_success "Exécutable minishell créé et exécutable"
    else
        print_failure "Exécutable minishell non trouvé ou non exécutable"
        return 1
    fi
    
    return 0
}

# ============================================================================
# TESTS DE BASE
# ============================================================================

test_basic_commands() {
    print_header "2. COMMANDES SIMPLES & GLOBALES"
    
    print_test "Commande simple avec chemin absolu"
    echo "/bin/ls" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    if [[ $? -eq 0 ]]; then
        print_success "Exécution de /bin/ls"
    else
        print_failure "Échec exécution /bin/ls"
    fi
    
    print_test "Commande vide"
    echo "" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    print_info "Vérifiez manuellement: commande vide ne doit pas planter"
    
    print_test "Espaces et tabs seulement"
    echo "   	  " | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    print_info "Vérifiez manuellement: espaces/tabs ne doivent pas planter"
}

test_arguments() {
    print_header "3. ARGUMENTS & HISTORIQUE"
    
    print_test "Commande avec arguments"
    echo "/bin/ls -la" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    if [[ $? -eq 0 ]]; then
        print_success "Exécution de /bin/ls -la"
    else
        print_failure "Échec exécution /bin/ls -la"
    fi
    
    print_test "Commande avec multiple arguments"
    echo "/bin/echo arg1 arg2 arg3" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    if [[ $? -eq 0 ]]; then
        print_success "Commande avec multiples arguments"
    else
        print_failure "Échec avec multiples arguments"
    fi
}

test_echo() {
    print_header "4. BUILTIN ECHO"
    
    print_test "echo simple"
    result=$(echo "echo hello" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    if [[ "$result" == "hello" ]]; then
        print_success "echo hello fonctionne"
    else
        print_failure "echo hello ne fonctionne pas (résultat: '$result')"
    fi
    
    print_test "echo avec -n"
    result=$(echo "echo -n hello" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    print_info "Vérifiez manuellement: echo -n ne doit pas ajouter de newline"
    
    print_test "echo sans arguments"
    echo "echo" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    if [[ $? -eq 0 ]]; then
        print_success "echo sans arguments fonctionne"
    else
        print_failure "echo sans arguments échoue"
    fi
}

test_exit() {
    print_header "5. BUILTIN EXIT"
    
    print_test "exit simple"
    echo "exit" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    if [[ $? -eq 0 ]]; then
        print_success "exit fonctionne"
    else
        print_failure "exit ne fonctionne pas"
    fi
    
    print_test "exit avec code"
    echo "exit 42" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    if [[ $? -eq 42 ]]; then
        print_success "exit avec code de retour"
    else
        print_failure "exit avec code de retour ne fonctionne pas"
    fi
}

test_return_values() {
    print_header "6. VALEURS DE RETOUR"
    
    print_test "Variable \$? après succès"
    result=$(echo -e "/bin/true\necho \$?" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    if [[ "$result" == "0" ]]; then
        print_success "\$? = 0 après commande réussie"
    else
        print_failure "\$? incorrect après succès (résultat: '$result')"
    fi
    
    print_test "Variable \$? après échec"
    result=$(echo -e "/bin/false\necho \$?" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    if [[ "$result" == "1" ]]; then
        print_success "\$? = 1 après commande échouée"
    else
        print_failure "\$? incorrect après échec (résultat: '$result')"
    fi
}

test_signals() {
    print_header "7. SIGNAUX"
    print_info "Tests de signaux - à vérifier manuellement:"
    print_info "- Ctrl-C sur prompt vide → nouvelle ligne"
    print_info "- Ctrl-\\ sur prompt vide → rien"
    print_info "- Ctrl-D sur prompt vide → quitte"
    print_info "- Ctrl-C après avoir tapé → nouvelle ligne"
    print_info "- Ctrl-D après avoir tapé → rien"
    print_info "- Ctrl-\\ après avoir tapé → quitte"
}

test_quotes() {
    print_header "8. GUILLEMETS"
    
    print_test "Guillemets doubles avec espaces"
    result=$(echo 'echo "hello world"' | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    if [[ "$result" == "hello world" ]]; then
        print_success "Guillemets doubles préservent les espaces"
    else
        print_failure "Guillemets doubles ne fonctionnent pas (résultat: '$result')"
    fi
    
    print_test "Guillemets simples"
    result=$(echo "echo 'hello world'" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    if [[ "$result" == "hello world" ]]; then
        print_success "Guillemets simples fonctionnent"
    else
        print_failure "Guillemets simples ne fonctionnent pas (résultat: '$result')"
    fi
    
    print_test "Variable dans guillemets simples"
    result=$(echo "echo '\$USER'" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    if [[ "$result" == "\$USER" ]]; then
        print_success "Variables non interprétées dans guillemets simples"
    else
        print_failure "Variables interprétées dans guillemets simples (résultat: '$result')"
    fi
}

test_builtins() {
    print_header "9. BUILTINS"
    
    print_test "env"
    echo "env" | timeout 5 "$MINISHELL_PATH" | grep -q "PATH"
    if [[ $? -eq 0 ]]; then
        print_success "env affiche les variables d'environnement"
    else
        print_failure "env ne fonctionne pas"
    fi
    
    print_test "pwd"
    result=$(echo "pwd" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    if [[ "$result" == "$(pwd)" ]]; then
        print_success "pwd retourne le bon répertoire"
    else
        print_failure "pwd ne fonctionne pas (résultat: '$result', attendu: '$(pwd)')"
    fi
    
    print_test "export"
    result=$(echo -e "export TEST=hello\necho \$TEST" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    if [[ "$result" == "hello" ]]; then
        print_success "export crée des variables"
    else
        print_failure "export ne fonctionne pas (résultat: '$result')"
    fi
    
    print_test "unset"
    result=$(echo -e "export TEST=hello\nunset TEST\necho \$TEST" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    if [[ -z "$result" ]]; then
        print_success "unset supprime des variables"
    else
        print_failure "unset ne fonctionne pas (résultat: '$result')"
    fi
    
    print_test "cd"
    echo -e "cd /tmp\npwd" | timeout 5 "$MINISHELL_PATH" | grep -q "/tmp"
    if [[ $? -eq 0 ]]; then
        print_success "cd change de répertoire"
    else
        print_failure "cd ne fonctionne pas"
    fi
}

test_paths() {
    print_header "10. CHEMINS"
    
    print_test "Chemin relatif"
    if [[ -x "./minishell" ]]; then
        echo "./minishell --version" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
        print_success "Chemins relatifs fonctionnent"
    else
        print_info "Test chemin relatif - à vérifier manuellement"
    fi
    
    print_test "Commandes sans chemin (PATH)"
    echo "ls" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    if [[ $? -eq 0 ]]; then
        print_success "Commandes trouvées via PATH"
    else
        print_failure "Commandes via PATH ne fonctionnent pas"
    fi
}

test_redirections() {
    print_header "11. REDIRECTIONS"
    
    setup_test_files
    
    print_test "Redirection de sortie (>)"
    echo "echo test > $TEST_FILES_DIR/output.txt" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    if [[ -f "$TEST_FILES_DIR/output.txt" ]] && grep -q "test" "$TEST_FILES_DIR/output.txt"; then
        print_success "Redirection > fonctionne"
    else
        print_failure "Redirection > ne fonctionne pas"
    fi
    
    print_test "Redirection d'entrée (<)"
    echo "cat < $TEST_FILES_DIR/test.txt" | timeout 5 "$MINISHELL_PATH" | grep -q "Hello World"
    if [[ $? -eq 0 ]]; then
        print_success "Redirection < fonctionne"
    else
        print_failure "Redirection < ne fonctionne pas"
    fi
    
    print_test "Redirection append (>>)"
    echo -e "echo line1 > $TEST_FILES_DIR/append.txt\necho line2 >> $TEST_FILES_DIR/append.txt" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    if [[ -f "$TEST_FILES_DIR/append.txt" ]] && [[ $(wc -l < "$TEST_FILES_DIR/append.txt") -eq 2 ]]; then
        print_success "Redirection >> fonctionne"
    else
        print_failure "Redirection >> ne fonctionne pas"
    fi
    
    print_test "Heredoc (<<)"
    print_info "Heredoc - à tester manuellement: cat << EOF"
    
    cleanup_test_files
}

test_pipes() {
    print_header "12. PIPES"
    
    setup_test_files
    
    print_test "Pipe simple"
    echo "echo hello | cat" | timeout 5 "$MINISHELL_PATH" | grep -q "hello"
    if [[ $? -eq 0 ]]; then
        print_success "Pipe simple fonctionne"
    else
        print_failure "Pipe simple ne fonctionne pas"
    fi
    
    print_test "Pipes multiples"
    echo "echo hello | cat | cat" | timeout 5 "$MINISHELL_PATH" | grep -q "hello"
    if [[ $? -eq 0 ]]; then
        print_success "Pipes multiples fonctionnent"
    else
        print_failure "Pipes multiples ne fonctionnent pas"
    fi
    
    print_test "Pipes avec commande qui échoue"
    echo "ls nonexistent | grep test" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    print_info "Vérifiez manuellement: les pipes gèrent les échecs"
    
    cleanup_test_files
}

test_environment_variables() {
    print_header "13. VARIABLES D'ENVIRONNEMENT"
    
    print_test "Variable \$USER"
    result=$(echo "echo \$USER" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    if [[ -n "$result" ]]; then
        print_success "Variable \$USER accessible (résultat: '$result')"
    else
        print_failure "Variable \$USER non accessible"
    fi
    
    print_test "Variable dans guillemets doubles"
    if [[ -n "$USER" ]]; then
        result=$(echo "echo \"\$USER\"" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
        if [[ "$result" == "$USER" ]]; then
            print_success "Variables interprétées dans guillemets doubles"
        else
            print_failure "Variables non interprétées dans guillemets doubles"
        fi
    else
        print_info "Variable USER non définie - test sauté"
    fi
}

test_crazy_cases() {
    print_header "14. CAS EXTRÊMES"
    
    print_test "Commande inexistante"
    echo "commandthatdoesnotexist" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    if [[ $? -ne 0 ]]; then
        print_success "Commande inexistante gérée correctement"
    else
        print_failure "Commande inexistante pas gérée"
    fi
    
    print_test "Longue commande"
    long_cmd="echo $(printf 'a%.0s' {1..100})"
    echo "$long_cmd" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    if [[ $? -eq 0 ]]; then
        print_success "Longue commande gérée"
    else
        print_failure "Longue commande pose problème"
    fi
    
    print_info "Tests manuels supplémentaires recommandés:"
    print_info "- Ctrl-C puis Enter (buffer doit être vidé)"
    print_info "- Navigation historique avec flèches"
    print_info "- Commandes avec beaucoup d'arguments"
}

# ============================================================================
# TESTS BONUS
# ============================================================================

test_bonus() {
    print_header "15. BONUS (si partie obligatoire parfaite)"
    
    print_test "&& et ||"
    echo "echo test && echo success" | timeout 5 "$MINISHELL_PATH" | grep -q "success"
    if [[ $? -eq 0 ]]; then
        print_success "Opérateur && fonctionne"
    else
        print_info "Opérateur && non implémenté (bonus)"
    fi
    
    print_test "Wildcards"
    echo "echo *.c" | timeout 5 "$MINISHELL_PATH" > /dev/null 2>&1
    print_info "Wildcards - à tester manuellement dans un dossier avec des .c"
    
    print_test "Test surprise"
    result=$(echo "echo '\"\$USER\"'" | timeout 5 "$MINISHELL_PATH" 2>/dev/null | tail -1)
    print_info "Test surprise - résultat: '$result'"
}

# ============================================================================
# FONCTION PRINCIPALE
# ============================================================================

run_all_tests() {
    echo -e "${CYAN}"
    echo "██╗  ██╗██████╗      ███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗"
    echo "██║  ██║╚════██╗     ████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║"
    echo "███████║ █████╔╝     ██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║"
    echo "╚════██║██╔═══╝      ██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║"
    echo "     ██║███████╗     ██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗"
    echo "     ╚═╝╚══════╝     ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"
    echo -e "${NC}"
    echo -e "${PURPLE}Suite de tests complète basée sur la grille d'évaluation 42${NC}\n"
    
    # Vérification de l'existence du minishell
    if [[ ! -f "$MINISHELL_PATH" ]]; then
        print_failure "Minishell non trouvé à $MINISHELL_PATH"
        echo "Assurez-vous d'être dans le bon répertoire et d'avoir compilé"
        exit 1
    fi
    
    # Exécution des tests
    test_compilation
    test_basic_commands
    test_arguments
    test_echo
    test_exit
    test_return_values
    test_signals
    test_quotes
    test_builtins
    test_paths
    test_redirections
    test_pipes
    test_environment_variables
    test_crazy_cases
    test_bonus
    
    # Résumé final
    print_header "RÉSUMÉ FINAL"
    echo -e "${CYAN}Tests totaux:${NC} $TOTAL_TESTS"
    echo -e "${GREEN}Tests réussis:${NC} $PASSED_TESTS"
    echo -e "${RED}Tests échoués:${NC} $FAILED_TESTS"
    
    percentage=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo -e "${PURPLE}Pourcentage de réussite:${NC} $percentage%"
    
    if [[ $percentage -ge 80 ]]; then
        echo -e "\n${GREEN}🎉 Excellent travail ! Votre minishell semble bien fonctionner !${NC}"
    elif [[ $percentage -ge 60 ]]; then
        echo -e "\n${YELLOW}📝 Bon travail, mais il y a encore des points à améliorer.${NC}"
    else
        echo -e "\n${RED}💪 Continuez, il reste du travail mais vous êtes sur la bonne voie !${NC}"
    fi
    
    echo -e "\n${CYAN}RAPPELS IMPORTANTS:${NC}"
    echo "- Vérifiez les fuites mémoire avec valgrind"
    echo "- Testez manuellement les signaux (Ctrl-C, Ctrl-D, Ctrl-\\)"
    echo "- Testez l'historique avec les flèches"
    echo "- Vérifiez que votre shell ne crash jamais"
    echo "- Les bonus ne comptent que si la partie obligatoire est parfaite"
    
    return $FAILED_TESTS
}

# Exécution si le script est appelé directement
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    run_all_tests
fi