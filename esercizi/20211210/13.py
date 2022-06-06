vf=[("Nella vecchia fattoria","Quante bestie ha zio Tobia","C'e' la capra","capra","ca"),
        ("Attaccato a un carrettino","C'è un quadrupede piccino","L'asinel","nel","nè"),
        ("Tra le casse e i ferri rotti", "Dove i topi son grassotti","C'è un bel gatto","gatto","ga"),
        ("Tanto grasso e tanto grosso", "Sempre sporco a più non posso", "C'è il maiale","iale","ia"),
        ("Poi sull'argine del fosso", "Alle prese con un osso", "C'è un bel cane","cane","ca"),
        ("Nella stalla silenziosa", "Dopo aver mangiato a iosa", "Dorme il bue","bue","bu")]
refrain="ia ia o"
 
def recverse(m,n):
        v1,v2,an1,an2,an3=vf[m]
        if n==m:
                print(v1,refrain)
                print(v2,refrain)
        else:
                recverse(m+1,n)
        print(an1,an2,an3,an3,an2)
 
for i in range(len(vf)):
        recverse(0,i)
        print(vf[0][0],refrain)
        print()
