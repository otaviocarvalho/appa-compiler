/* Número primo */

int main()
{
  int numero;
  bool prim;
  
  //Imprime mensagem para o operador solicitando um valor
  output "Informe um número: ";
  
  //Lê o valor informado pelo operador  
  input numero;
  
  //Faz chamada de função com número recebido
  prim = primo(numero);  
  
  //Testa retorno da chamada de função e imprime mensagem na tela
  if (prim == true) then
  {
    output "É primo"; 
  }    
  else
  {
    output "Não é primo";
  };
}

bool primo (int num) 
{
  int meio;
  int result;
  bool eh_primo;
  
  meio = num / 2;
  eh_primo = true;
  
  //Todos os números primos são >= 2
  if (num < 2) then
  {
    return false;
  };
  
  //Testa a divisão para todos os números <= a metade do número informado
  while (meio > 1 && eh_primo == true) do 
  {
    result = num / meio;
    
    //Teste é valido pois result é do tipo int
    if((result * meio) == numero) then
    {
      eh_primo = false;  
    };
    
    meio = meio - 1;
  };
  return eh_primo;
}

