from Crypto.Util.number import long_to_bytes, bytes_to_long
import secrets



def ROTL(a, b):
    return (((a) << (b)) | ((a % 2**32) >> (32 - (b)))) % 2**32

def qr(x, a, b, c, d):
    x[a] += x[b]; x[d] ^= x[a]; x[d] = ROTL(x[d],16)
    x[c] += x[d]; x[b] ^= x[c]; x[b] = ROTL(x[b],12)
    x[a] += x[b]; x[d] ^= x[a]; x[d] = ROTL(x[d], 8)
    x[c] += x[d]; x[b] ^= x[c]; x[b] = ROTL(x[b], 7)

ROUNDS = 20

def chacha_block(inp):
    x = list(inp)
    for i in range(0, ROUNDS, 2):
        qr(x, 0, 4, 8, 12)
        qr(x, 1, 5, 9, 13)
        qr(x, 2, 6, 10, 14)
        qr(x, 3, 7, 11, 15)

        qr(x, 0, 5, 10, 15)
        qr(x, 1, 6, 11, 12)
        qr(x, 2, 7, 8, 13)
        qr(x, 3, 4, 9, 14)

    return [(a+b) % 2**32 for a, b in zip(x, inp)]


state = [0] * 16
buffer = b""


def encrypt(data):
    global state, buffer

    
    print("buffer: ", len(buffer))
    output = []
    for b in data:
        if len(buffer) == 0:
            buffer = b"".join(long_to_bytes(x).rjust(4, b"\x00") for x in state)
            state = chacha_block(state)
            print("state: ", state)
        output.append(b ^ buffer[0])
        buffer = buffer[1:]
    return bytes(output)


def result_to_state(result, string):


    output = []
    for i in range(64):
        output.append(result[i] ^ string[i])



    return bytes(output)



def format_state(decoded):
    
    state = [0] * 16
    for i in range(16):
        state[i] = bytes_to_long(decoded[i*4:(i+1)*4])

    return state






if __name__ == "__main__":


    base = b"0000000000000000000000000000000000000000000000000000000000000000"

    result_hex = "514048550310545e49521d025b10554442dd2d8bad329d288f187f3f7d5e86603c36d6d893574b55ba8e8c9c2f53b4c83030303030303030ff2ee32fc744b4aa"

    result = bytes.fromhex(result_hex)

    decoded = result_to_state(result, base)

    state = format_state(decoded)

    print(state)

    #print(decoded)

    encrypted_msg = encrypt(base)



    flag_enc_hex = '92d1ccd6b9f4a479025919372c5b5cc040d0b75b9d98db296a956d475b8c332c204097f7d4'

    flag_enc = bytes.fromhex(flag_enc_hex)

    print(encrypt(flag_enc))

    #print(encrypted_msg.hex())

    #print(state)

    


