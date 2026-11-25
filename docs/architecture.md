Server Python 3.12 + FastAPI + uvicorn

Endpoints

Method	Path	Purpose
POST	/api/v1/readings	Receive a reading from a node
GET	/api/v1/nodes	List known nodes and last-seen times
GET	/api/v1/readings?node=&from=&to=	Query data for charts
GET	/	Serve the dashboard page