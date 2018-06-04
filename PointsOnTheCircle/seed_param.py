
for seed in range(8001,8101):

    path = 'D:\\dev\\PointsOnTheCircle\\cache\\5010793b5d5a534c85f2740f800f7a83\\' + str(seed) + '_stderr'

    with open(path,'r') as inp:
        s = inp.readline()
        tokens = s.rstrip('\n').split('\t')
        for token in tokens:
            t = token.split(' ')
            if t[0] == 'N':
                print(t[2],end='\t')
            else:
                print(t[2],end='\n')
    
