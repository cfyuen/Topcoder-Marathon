tags = ['a2005']

for tag in tags:

    path = 'D:\\dev\\PointsOnTheCircle\\tags\\' + tag + '.score'

    with open(path,'r') as inp:
        for line in inp:
            tokens = line.rstrip('\n').split('\t')
            for token in tokens:
                t = token.split(' ')
                t = list(filter(None,t))
                # print(t)
                if t[0] == 'Run':
                    continue
                elif t[0] == 'Seed:':
                    print(t[-1],end='\t')
                else:
                    print(t[-1],end='\n')
        print('\n')
        
