# parallelized-dc

<h1 align = "center"> A Parallel Graph Partitioning Approach to Enhance Community Detection in Social Networks </h1>


The need to detect communities in social networks has always been challenging, especially when analyzing large networks. Therefore, I present a **new partitioning algorithm** based on what I call a **subtree-splitting strategy**. The algorithm was designed to work on density-based algorithms such as NetSCAN or DBSCAN. The algorithms' goal is to split a graph structure into _n_ smaller components with respect to the following particulars:

* The partitioning process keeps the partition's connectivity.
* The algorithm strategy also allows for the overlapping of nodes between partitions. 

## Database
The Digital Bibliography& Library Project (DBLP) database was modeled as a scientific citation network used to support the analysis and experiments.
* DBLP database instance: https://drive.google.com/open?id=1JzwoY3sHyE5sFma7vAgxORYTU8faInWU
* Smaller instances: ttps://github.com/Talessil/Instances

## Published Work
This work was published in the **25th IEEE Symposium on Computers and Communications (ISCC)**.
* Youtube presentation link: https://youtu.be/o234YyG6xf0
* Article link: https://ieeexplore.ieee.org/abstract/document/9219602
