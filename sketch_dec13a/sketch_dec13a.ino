/* 
 *  EJEMPLO DE USO DE STRTOK
 *  
 *  Separa un String por campos delimitados por un caracter.
 *  
 */

// Campos contiene la cadena formateada, con separador ",".
String campos = "34.3434,12.1212,99.9909";

// s en un puntero auxiar necesario.
char *s;

void setup() {
  // Iniciar el puerto serie.
  Serial.begin(9600);
  // En la primera pasa strtok devuelve el string hasta el primer caracter
  // delimitador.
  // Nota: La función strtok es una función estándar de la librería de 
  //       C++, pero no es un miembro clase de String, por lo que no se
  //       puede usar con objetos String. Hay que utilizar la función
  //       miembro c_str() para poder acceder al puntero de char del
  //       objeto String.
  s = strtok(campos.c_str(), ",");
  // Para encontrar todos hay que seguir llamando a strtok pasando como 
  // parametro NULL en la cadena a buscar, cuando haya hallado todos los
  // campos separados por "," devolverá NULL.
  while (s!=NULL) {
    Serial.println(s);
    s=strtok(NULL, ",");
  }
}

void loop() {

}
