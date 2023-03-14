import requests
import pyodbc
import time    
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import pandas as pd

def sinal():
    proxies = {'https': 'http://disrct:etsds10243110@rb-proxy-de.bosch.com:8080'}
    url_temperatura = 'https://prova-47a0e-default-rtdb.firebaseio.com/andre/Sensor/temperatura.json'
    url_umidade = 'https://prova-47a0e-default-rtdb.firebaseio.com/andre/Sensor/umidade.json'
    temperatura = float(requests.get(url_temperatura, proxies=proxies).content)
    umidade = float(requests.get(url_umidade, proxies=proxies).content)
    return temperatura, umidade

def InserirBD(sinal):
    cursor = ConectarBD()
    #cursor.execute("CREATE TABLE Sensor(Temperatura float, Umidade float, Tempo datetime default getdate())")
    cursor.execute(f"INSERT Sensor (Temperatura, Umidade) VALUES ({sinal[0]},{sinal[1]});")
    cursor.commit()
    print("Inserido com sucesso!")
    
def apresentar(sinal):
    print(f"Temperatura: {sinal[0]}")
    print(f"Umidade: {sinal[1]}")

def ConectarBD():
    server = 'CT-C-0013J\SQLEXPRESS01'
    database = 'prova'
    cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER='+server+';DATABASE='+database+';Trusted_Connection=yes;')
    cursor = cnxn.cursor()
    return cursor
    
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
        row = cursor.fetchone()
    Grafico(listatemperatura, listatempo, listaumidade)
    cursor.close()
    cursor = ConectarBD()
    cursor.execute("SELECT Temperatura, Tempo, Umidade FROM dbo.Sensor")
    linhas = len(cursor.fetchall())
    print("Linhas inseridas: ", linhas)
    
def Grafico(listatemperatura, listatempo, listaumidade):    
    df=pd.DataFrame({"Temperatura":listatemperatura, "Tempo":listatempo, "Umidade":listaumidade})
    df['Tempo'] = pd.to_datetime(df["Tempo"])
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
    time.sleep(30)
    
    