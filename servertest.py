# coding:utf-8

import socket

from multiprocessing import Process
import func
import xlrd

def getdata(lineindex):
    dataworkbook=xlrd.open_workbook('vocabulary.xls')
    datatable=dataworkbook.sheet_by_index(0)
    word=datatable.cell_value(lineindex,0)
    pron=datatable.cell_value(lineindex,1)
    part_speech=datatable.cell_value(lineindex,2)
    chs=datatable.cell_value(lineindex,3)
    return word,pron,part_speech,chs




def handle_client(client_socket,text):
    #with open('a','rb') as f:
        #text=f.read()
    """
    处理客户端请求
    """
    request_data = client_socket.recv(1024)
    print("request data:", str(request_data,encoding='utf-8'))
    # 构造响应数据
    request_list=request_data.split('\n')
    request_line=request_list[0]
    request_file=request_line[14,-10]
    print(request_file)
    response_start_line = "HTTP/1.1 200 OK\r\n"
    response_headers = "Server: My server\r\n"
    #response_body = "shit"
    response =bytes( response_start_line + response_headers + "\r\n",encoding='utf-8') + text

    # 向客户端返回响应数据
    client_socket.send(response)

    # 关闭客户端连接
    client_socket.close()


if __name__ == "__main__":
    name=socket.gethostname()
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((name, 8005))
    server_socket.listen(128)

    while True:
        client_socket, client_address = server_socket.accept()
        print("[%s, %s]user connected" % client_address)
        handle_client_process = Process(target=handle_client, args=(client_socket,))
        handle_client_process.start()
        client_socket.close()
