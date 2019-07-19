import csv
import json
import sys
import datetime
import json
import urllib3
import time

INDEX_NAME = 'legoheads_clawbot_metrics'

# so python doesn't nag us about unverified SSL creds
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

from elasticsearch import Elasticsearch
from elasticsearch.helpers import bulk

# es endpoint: https://search-clawbot-yb6lnvvznbyzzslex3vnp62sii.us-east-1.es.amazonaws.com/

# /usr/local/opt/python/libexec/bin/python -u CandyClawConvert.py ClawBot.CDR

# kibana: https://search-clawbot-yb6lnvvznbyzzslex3vnp62sii.us-east-1.es.amazonaws.com/_plugin/kibana/

# amps: https://search-clawbot-yb6lnvvznbyzzslex3vnp62sii.us-east-1.es.amazonaws.com/_plugin/kibana/app/kibana#/visualize/edit/228c35c0-cb4b-11e8-ae84-a789de9cf154?_g=(refreshInterval%3A(display%3AOff%2Cpause%3A!f%2Cvalue%3A0)%2Ctime%3A(from%3A'2018-10-07T00%3A00%3A00.000Z'%2Cmode%3Aabsolute%2Cto%3A'2018-10-07T03%3A00%3A00.000Z'))

# also: https://search-clawbot-yb6lnvvznbyzzslex3vnp62sii.us-east-1.es.amazonaws.com/_plugin/kibana/app/kibana#/dashboard/feeb9d80-cb4b-11e8-ae84-a789de9cf154?_g=(refreshInterval%3A(display%3AOff%2Cpause%3A!f%2Cvalue%3A0)%2Ctime%3A(from%3A'2018-10-07T00%3A00%3A00.000Z'%2Cmode%3Aabsolute%2Cto%3A'2018-10-07T03%3A00%3A00.000Z'))

# connect to our ES instance:
es_client = Elasticsearch('https://search-clawbot-yb6lnvvznbyzzslex3vnp62sii.us-east-1.es.amazonaws.com/', verify_certs=False)
print('Elasticsearch cluster: %s' % es_client.info())

def generate_documents(dataIn):

  count = 0

  start_time = time.time()
  
  with open(dataIn, 'r') as csvFile:

    # skip header info until we find the actual data
    while True:
      line = csvFile.readline()
      if line.startswith('Milliseconds'):
        break

    csvReader = csv.reader(csvFile, delimiter = ' ')

    # fake a start time:
    # tStart = datetime.datetime.now()
    tStart = datetime.datetime(year = 2018, month = 10, day = 6, hour = 20)
    print("Start time is: " + str(tStart))

    t_start_msec_epoch = tStart.timestamp() * 1000

    for row in csvReader:
      volts = float(row[8]) / 100
      amps = float(row[9]) / 100
      seconds = float(row[0]) / 1000

      # the metrics we will index for this document
      doc = {}
      doc['volts'] = volts
      doc['amps'] = amps
      #doc['timestamp'] = (tStart + datetime.timedelta(seconds = seconds)).isoformat(' ')

      # milliseconds since epoch:
      doc['timestamp'] = int(t_start_msec_epoch + seconds * 1000)
      #doc['timestamp'] = 'foobar'

      # the ES action, telling ES how to index our document
      if False:
        action = {}
        action['doc'] = doc
      else:
        action = doc
      action['_id'] = str(count)
      action['_op_type'] = 'create'

      yield action

      count += 1

      if count % 1000 == 0:
        print('%s documents... %.1f docs/sec' % (count, count / (time.time() - start_time)))

dataIn = sys.argv[1]

# delete old index, if present
es_client.indices.delete(index=INDEX_NAME, ignore=[400, 404])

# create new one, with our field mappings
mapping = '''
{  
  "mappings":{
    "doc":{  
      "dynamic": false,
      "properties":{  
        "volts":{  
          "type":"float"
        },
        "amps":{  
          "type":"float"
        },
        "timestamp":{  
          "type":"date",
          "format": "epoch_millis"
        }
      }
    }
  }
}'''

#creating initial index (empty)
es_client.indices.create(index=INDEX_NAME, body=mapping)

#sending stream over to elasticsearch
bulk(es_client, generate_documents(dataIn), index=INDEX_NAME, doc_type='doc')
