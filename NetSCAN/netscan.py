from neo4j import GraphDatabase
import pandas as pd
import csv
import time
graphdb = GraphDatabase.driver(uri="bolt://localhost:7687",auth=("neo4j", "1234"))


if __name__ == "__main__":

    start_time = time.time()
	
    session = graphdb.session() 
	session.run("Load csv with headers from 'file:/graph.csv' as csvline MERGE (u:Pessoa {idpessoa: toInteger(csvline.author_id1)})  ON CREATE SET u.idpessoa = toInteger(csvline.author_id1)")
	session.run("Load csv with headers from 'file:/graph.csv' as csvline MERGE (u:Pessoa {idpessoa: toInteger(csvline.author_id2)})  ON CREATE SET u.idpessoa = toInteger(csvline.author_id2)")
	session.run("Load csv with headers from 'file:/graph.csv' as csvline MATCH (a:Pessoa {idpessoa: toInteger(csvline.author_id1)}),(b:Pessoa {idpessoa: toInteger(csvline.author_id2)}) create (a)-[r:Publicou{total:toFloat(csvline.count)}]->(b)")
	session.close()
	
	session = graphdb.session()
	session.run("CALL netscan.find_communities('Pessoa','Publicou','idpessoa','total', 0.5, 5, 1)")
	session.close()

    print("--- %s seconds ---" % (time.time() - start_time))
	