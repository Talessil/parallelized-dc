from neo4j import GraphDatabase
import pandas as pd
import csv
import subprocess
import time
graphdb = GraphDatabase.driver(uri="bolt://localhost:7687",auth=("neo4j", "1234"))


class AdjNode:
    def __init__(self, value, w):
        self.vertex = value
        self.weight = w
        self.next = None


class Graph:
    def __init__(self, num):
        self.V = num
        self.graph = [None] * self.V

    # Add edges (colocar peso)
    def add_edge(self, s, d, w):
        node = AdjNode(d,w)
        node.next = self.graph[s]
        self.graph[s] = node

    def check_edge(self,a,b):
    	temp = self.graph[a]
    	while temp:
    		if temp.vertex == b:
    			return 1
    		else:
    			temp = temp.next
    	return 0

    def get_weight(self,a,b):
    	temp = self.graph[a]
    	while temp:
    		if temp.vertex == b:
    			return temp.weight
    		else:
    			temp = temp.next

    # Print the graph
    def print_agraph(self):
        for i in range(self.V):
            print("Vertex " + str(i) + ":", end="")
            temp = self.graph[i]
            while temp:
                print(" -> {}".format(temp.vertex), end="")
                temp = temp.next
            print(" \n")

    def merge(self,i,ite,aux):
    	dados =  pd.read_csv(i[ite], sep=",", header=0)
    	array = dados.values
    	k = 0
    	for n in array:
    		a = int(array[k][0])
    		b = int(array[k][1])
    		k = k + 1
    		l = 0 #position
    		for n2 in aux:
    			if n2 == 1:
    				if self.check_edge(l,a):
    					# (I) insere as arestas (merge). (II) remove noise,expanded,core. (III) remove ligação com grupos.
    					session = graphdb.session() 
    					session.run("MATCH (a:Pessoa),(b:Pessoa) WHERE a.idpessoa = $a_id And b.idpessoa = $b_id MERGE (a)-[r:Publicou{total:toFloat($count)}]->(b)", a_id = l, b_id = a, count = self.get_weight(l,a))
    					session.run("MATCH (a:Pessoa),(b:Pessoa) WHERE a.idpessoa = $a_id And b.idpessoa = $b_id REMOVE a.noise, a.expanded, a.core, b.noise, b.expanded, b.core", a_id = l, b_id = a)
    					#session.run("match (c:Cluster)-[b:CONTAINS]->(a:Pessoa{idpessoa:$a_id}) delete b", a_id = l)
    					#session.run("match (c:Cluster)-[b:CONTAINS]->(a:Pessoa{idpessoa:$b_id}) delete b", b_id = a)
    					session.close()
    				if self.check_edge(a,l):
    					session = graphdb.session() 
    					session.run("MATCH (a:Pessoa),(b:Pessoa) WHERE a.idpessoa = $a_id And b.idpessoa = $b_id MERGE (a)-[r:Publicou{total:toFloat($count)}]->(b)", a_id = a, b_id = l, count = self.get_weight(a,l))
    					session.run("MATCH (a:Pessoa),(b:Pessoa) WHERE a.idpessoa = $a_id And b.idpessoa = $b_id REMOVE a.noise, a.expanded, a.core, b.noise, b.expanded, b.core", a_id = a, b_id = l)
    					#session.run("match (c:Cluster)-[b:CONTAINS]->(a:Pessoa{idpessoa:$a_id}) delete b", a_id = a)
    					#session.run("match (c:Cluster)-[b:CONTAINS]->(a:Pessoa{idpessoa:$b_id}) delete b", b_id = l)
    					session.close()
    				if self.check_edge(l,b):
    					session = graphdb.session() 
    					session.run("MATCH (a:Pessoa),(b:Pessoa) WHERE a.idpessoa = $a_id And b.idpessoa = $b_id MERGE (a)-[r:Publicou{total:toFloat($count)}]->(b)", a_id = l, b_id = b, count = self.get_weight(l,b))
    					session.run("MATCH (a:Pessoa),(b:Pessoa) WHERE a.idpessoa = $a_id And b.idpessoa = $b_id REMOVE a.noise, a.expanded, a.core, b.noise, b.expanded, b.core", a_id = l, b_id = b)
    					#session.run("match (c:Cluster)-[b:CONTAINS]->(a:Pessoa{idpessoa:$a_id}) delete b", a_id = l)
    					#session.run("match (c:Cluster)-[b:CONTAINS]->(a:Pessoa{idpessoa:$b_id}) delete b", b_id = b)
    					session.close()
    				if self.check_edge(b,l):
    					session = graphdb.session() 
    					session.run("MATCH (a:Pessoa),(b:Pessoa) WHERE a.idpessoa = $a_id And b.idpessoa = $b_id MERGE (a)-[r:Publicou{total:toFloat($count)}]->(b)", a_id = b, b_id = l, count = self.get_weight(b,l))
    					session.run("MATCH (a:Pessoa),(b:Pessoa) WHERE a.idpessoa = $a_id And b.idpessoa = $b_id REMOVE a.noise, a.expanded, a.core, b.noise, b.expanded, b.core", a_id = b, b_id = l)
    					#session.run("match (c:Cluster)-[b:CONTAINS]->(a:Pessoa{idpessoa:$a_id}) delete b", a_id = b)
    					#session.run("match (c:Cluster)-[b:CONTAINS]->(a:Pessoa{idpessoa:$b_id}) delete b", b_id = l)
    					session.close()
    			l = l + 1
    	k = 0
    	for n in array:
    		a = int(array[k][0])
    		b = int(array[k][1])
    		aux[a] = 1
    		aux[b] = 1
    		k = k + 1


if __name__ == "__main__":

    start_time = time.time()

        # CALL C PROGRAM: PARTITIONING
    subprocess.call(["./temp", "4"])

		# SAVE GRAPH INTO A ADJACENCY LIST

    dados =  pd.read_csv('graph.csv', sep=",", header=0)
    array = dados.values
    V = 22 #1306563 dblp (sempre o maior id + 1)
    graph = Graph(V)
    k = 0
    for n in array:
        graph.add_edge(int(array[k][0]),int(array[k][1]),float(array[k][2]))
        k = k + 1

		# AUX ARRAY (set the nodes inserted into the graph)

    aux = V * [None]
    for k in range(V):
        aux[k] = 0

		#LOAD FILES
	
    i = ['file0.csv', 'file1.csv', 'file2.csv', 'file3.csv']
    ii = ['file:/file0.csv', 'file:/file1.csv', 'file:/file2.csv', 'file:/file3.csv']

		#FIRST FILE: SAVE IN AUX THE VALUES

    dados =  pd.read_csv(i[0], sep=",", header=0)
    array = dados.values
    k = 0
    for n in array:
    	a = int(array[k][0])
    	b = int(array[k][1])
    	aux[a] = 1
    	aux[b] = 1
    	k = k + 1
	
	
	
		#FIRST FILE
	
    session = graphdb.session() 	
    session.run("Load csv with headers from $url as csvline MERGE (u:Pessoa {idpessoa: toInteger(csvline.author_id1)})  ON CREATE SET u.idpessoa = toInteger(csvline.author_id1)", url = ii[0])
    session.run("Load csv with headers from $url as csvline MERGE (u:Pessoa {idpessoa: toInteger(csvline.author_id2)})  ON CREATE SET u.idpessoa = toInteger(csvline.author_id2)", url = ii[0])
    session.run("Load csv with headers from $url as csvline MATCH (a:Pessoa {idpessoa: toInteger(csvline.author_id1)}),(b:Pessoa {idpessoa: toInteger(csvline.author_id2)}) create (a)-[r:Publicou{total:toFloat(csvline.count)}]->(b)", url = ii[0])
    session.close()
	
    session = graphdb.session()
    session.run("CALL netscan.find_communities('Pessoa','Publicou','idpessoa','total', 0.5, 5, 1)") #epsilon, minPnt, raio (protein: 0.5,5,1)
    session.close()
	

		#OTHER FILES
    for ite in range(1,len(ii)):
    	session = graphdb.session() 
    	session.run("Load csv with headers from $url as csvline MERGE (u:Pessoa {idpessoa: toInteger(csvline.author_id1)})  ON CREATE SET u.idpessoa = toInteger(csvline.author_id1)", url = ii[ite])
    	session.run("Load csv with headers from $url as csvline MERGE (u:Pessoa {idpessoa: toInteger(csvline.author_id2)})  ON CREATE SET u.idpessoa = toInteger(csvline.author_id2)", url = ii[ite])
    	session.run("Load csv with headers from $url as csvline MATCH (a:Pessoa {idpessoa: toInteger(csvline.author_id1)}),(b:Pessoa {idpessoa: toInteger(csvline.author_id2)}) create (a)-[r:Publicou{total:toFloat(csvline.count)}]->(b)", url = ii[ite])
    	session.close()
		
    	session = graphdb.session()
    	session.run("CALL netscan.find_communities('Pessoa','Publicou','idpessoa','total', 0.5, 5, 1)")
    	session.close()

    	graph.merge(i,ite,aux) 
		
    	session = graphdb.session()
    	session.run("CALL netscan.find_communities('Pessoa','Publicou','idpessoa','total', 0.5, 5, 1)")
    	session.close()
	
    

    print("--- %s seconds ---" % (time.time() - start_time))
	
		# ORIGINAL FILE
    """
	session = graphdb.session() 
	session.run("Load csv with headers from 'file:/graph.csv' as csvline MERGE (u:Pessoa {idpessoa: toInteger(csvline.author_id1)})  ON CREATE SET u.idpessoa = toInteger(csvline.author_id1)")
	session.run("Load csv with headers from 'file:/graph.csv' as csvline MERGE (u:Pessoa {idpessoa: toInteger(csvline.author_id2)})  ON CREATE SET u.idpessoa = toInteger(csvline.author_id2)")
	session.run("Load csv with headers from 'file:/graph.csv' as csvline MATCH (a:Pessoa {idpessoa: toInteger(csvline.author_id1)}),(b:Pessoa {idpessoa: toInteger(csvline.author_id2)}) create (a)-[r:Publicou{total:toFloat(csvline.count)}]->(b)")
	session.close()
	
	session = graphdb.session()
	session.run("CALL netscan.find_communities('Pessoa','Publicou','idpessoa','total', 0.5, 5, 1)")
	session.close()
	"""