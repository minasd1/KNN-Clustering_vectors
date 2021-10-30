//RETURNS a MODULO b SO THAT IT IS ALWAYS POSITIVE
//E.G. -10%11 = std::fmod(-10,11) = -1 BUT USING THIS FUNCTION THE RESULT IS -1+11=10
int mod(int a, int b)
{
    int result;

    result= a%b;
    if (result < 0) {
        result+= b;
    }
    return result;
}
