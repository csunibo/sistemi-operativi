def pf(n):
        f = []
        for i in range(2,n+1):
                while n % i == 0:
                        f.append(i)
                        n //= i
                if n == 1:
                        break
        return f
 
if __name__ == "__main__":
        n = int(input("dammi un numero (consigliato da 2 a 1000): "))
        print("il risultato della funzione misteriosa e' ", pf(n))
