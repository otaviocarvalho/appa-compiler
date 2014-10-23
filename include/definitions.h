#define IKS_TYPE_NOT_DEFINED -1
#define IKS_INT 0
#define IKS_FLOAT 1
#define IKS_CHAR 2
#define IKS_STRING 3
#define IKS_BOOL 4

#define TAMANHO_INT 4
#define TAMANHO_FLOAT 8
#define TAMANHO_CHAR 1
#define TAMANHO_STRING 1
#define TAMANHO_BOOL 1

#define IKS_SUCCESS 0
#define IKS_ERROR_UNDECLARED 1
#define IKS_ERROR_DECLARED 2
#define IKS_ERROR_VARIABLE 3
#define IKS_ERROR_VECTOR 4
#define IKS_ERROR_FUNCTION 5
#define IKS_ERROR_WRONG_TYPE 6
#define IKS_ERROR_STRING_TO_X 7
#define IKS_ERROR_CHAR_TO_X 8
#define IKS_ERROR_MISSING_ARGS 9
#define IKS_ERROR_EXCESS_ARGS 10
#define IKS_ERROR_WRONG_TYPE_ARGS 11
#define IKS_ERROR_WRONG_PAR_INPUT 12
#define IKS_ERROR_WRONG_PAR_OUTPUT 13
#define IKS_ERROR_WRONG_PAR_RETURN 14

#define DECLARACAO_VETOR_INDEXADO 1
#define USO_VETOR_INDEXADO 2
#define DECLARACAO_VARIAVEL 3
#define USO_VARIAVEL 4
#define DECLARACAO_FUNCAO 5
#define USO_FUNCAO 6
#define USO_LITERAL 7

#define JA_EXISTE 0
#define NAO_EXISTE 1

#define IDENTIFICADOR_DECLARADO 1
#define IDENTIFICADOR_NAO_DECLARADO -1

#define TAC_LABEL 30
#define TAC_GOTO 31
#define TAC_JUMP 32
#define TAC_ATRIBUICAO 33

#define INTERNO 1
#define EXTERNO 0