/* MDC de dois números */

int main()
{
  int num1;
  int num2;
  int resto;
  int aux;
  int divisor;
  int dividendo;
  
  //lê as entradas
  output "Informe o primeiro número: ";
  input num1;
  
  output "Informe o segundo número: ";
  input num2;
  
  //coloca no dividendo o maior valor
  if(num1 > num2) then
  {
    dividendo = num1;
    divisor = num2;
  }
  else
  {
    dividendo = num2;
    divisor = num1;
  }
  
  //encontra o resto da divisão
  aux = (dividendo / divisor);
  resto = dividendo - (divisor * aux);
  
  while (resto != 0) do
  {
    dividendo = divisor;
    divisor = resto;
      
    aux = dividendo / divisor;
    resto = dividendo - (divisor * aux);
  }
  
  output "O MDC dos números informados é: ", divisor; 
  
  return 0;
}