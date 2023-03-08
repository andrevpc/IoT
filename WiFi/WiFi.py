import requests
import json

proxies = {'https':"http://disrct:etsds10243110@10.224.200.26:8080"}
# r = requests.get('https://www.google.com.br/', proxies=proxies)
# print(r)

'''
url = 'https://random.dog/woof.json'
img_data = requests.get(url,proxies=proxies).content

new_url = json.loads(img_data)['url']
img = requests.get(new_url,proxies=proxies).content

with open('dog.jpg','wb') as dog_file:
    dog_file.write(img)
    
url_temperatura = 'https://teste-1cdf0-default-rtdb.firebaseio.com/andre/Sensor/temperatura.json'

url_umidade = 'https://teste-1cdf0-default-rtdb.firebaseio.com/andre/Sensor/umidade.json'

temperatura = float(requests.get(url_temperatura, proxies=proxies).content)
umidade = float(requests.get(url_umidade, proxies=proxies).content)
print(f"Temperatura: {temperatura} Umidade: {umidade} ")
'''

'''
nome = input("Insira o seu nome: ")
url = f'https://api.agify.io?name={nome}'
nome_data = requests.get(url,proxies=proxies).content

new_url = json.loads(nome_data)['age']

print(new_url)
'''

import pyodbc
import time    
import matplotlib.pyplot as plt
import pyodbc
import numpy as np
import seaborn as sns
import pandas as pd

def sinal():
    proxies = {'https': 'http://disrct:etsds10243110@rb-proxy-de.bosch.com:8080'}
    url_temperatura = 'https://teste-1cdf0-default-rtdb.firebaseio.com/andre/Sensor/temperatura.json'
    url_umidade = 'https://teste-1cdf0-default-rtdb.firebaseio.com/andre/Sensor/umidade.json'
    temperatura = float(requests.get(url_temperatura, proxies=proxies).content)
    umidade = float(requests.get(url_umidade, proxies=proxies).content)
    return temperatura, umidade

def ConectarBD():
    server = 'CT-C-0013J\SQLEXPRESS01'
    database = 'iot'
    cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER='+server+';DATABASE='+database+';Trusted_Connection=yes;')
    cursor = cnxn.cursor()
    return cursor

def InserirBD(sinal):
    cursor = ConectarBD()
    # cursor.execute("CREATE TABLE Sensor(Temperatura float, Umidade float, Tempo datetime not null default getdate())")
    cursor.execute(f"INSERT Sensor (Temperatura, Umidade) VALUES ({sinal[0]},{sinal[1]});")
    cursor.commit()
    print("Inserido com sucesso!")
    
def SelectBD():
    cursor = ConectarBD()
    cursor.execute("SELECT Temperatura, Tempo, Umidade FROM dbo.Sensor")
    row = cursor.fetchone()
    listatemperatura=[]
    listatempo=[]
    listaumidade=[]
    while row:
        listatemperatura.append(row[0])
        listatempo.append(str(row[1]))
        listaumidade.append(row[2])
        print(row)
        row = cursor.fetchone()
    Grafico(listatemperatura, listatempo, listaumidade)
    
def apresentar(sinal):
    print(f"Temperatura: {sinal[0]}")
    print(f"Umidade: {sinal[1]}")
    
def Grafico(listatemperatura, listatempo, listaumidade):    
    df=pd.DataFrame({"Temperatura":listatemperatura, "Tempo":listatempo, "Umidade":listaumidade})
    df['Tempo'] = pd.to_datetime(df["Tempo"])
    print(df)
    sns.relplot(x="Tempo", y="Temperatura", data=df, kind="line")
    plt.title("Temperatura por Tempo")
    plt.xticks(rotation=90)
    sns.relplot(x="Tempo", y="Umidade", data=df, kind="line")
    plt.title("Umidade por Tempo")
    plt.xticks(rotation=90)
    plt.show()

while True:
    valores=sinal()
    apresentar(valores)
    InserirBD(valores)
    SelectBD()
    time.sleep(120)
    
    