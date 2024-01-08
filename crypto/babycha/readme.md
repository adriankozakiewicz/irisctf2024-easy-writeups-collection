# babycha - irisctf 2024 writeup

Challage might look complex but after some analysis and testing it's not that bad.

```python

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

def chacha_init(key, nonce, counter):
    assert len(key) == 32
    assert len(nonce) == 8

    state = [0 for _ in range(16)]
    state[0] = bytes_to_long(b"expa"[::-1])
    state[1] = bytes_to_long(b"nd 3"[::-1])
    state[2] = bytes_to_long(b"2-by"[::-1])
    state[3] = bytes_to_long(b"te k"[::-1])

    key = bytes_to_long(key)
    nonce = bytes_to_long(nonce)

    for i in range(8):
        state[i+4] = key & 0xffffffff
        key >>= 32

    state[12] = (counter >> 32) & 0xffffffff
    state[13] = counter & 0xffffffff
    state[14] = (nonce >> 32) & 0xffffffff
    state[15] = nonce & 0xffffffff

    return state

state = chacha_init(secrets.token_bytes(32), secrets.token_bytes(8), 0)
buffer = b""


def encrypt(data):
    global state, buffer
    print("buffer: ", len(buffer))
    output = []
    for b in data:
        if len(buffer) == 0:
            buffer = b"".join(long_to_bytes(x).rjust(4, b"\x00") for x in state)
            state = chacha_block(state)
        output.append(b ^ buffer[0])
        buffer = buffer[1:]
    return bytes(output)



def pre(data):
    state2 = [0] * 16

    state2[0] = bytes_to_long(b"expa"[::-1])
    state2[1] = bytes_to_long(b"nd 3"[::-1])
    state2[2] = bytes_to_long(b"2-by"[::-1])
    state2[3] = bytes_to_long(b"te k"[::-1])




flag = b"fake_flag{FAKE_FLAG}"

if __name__ == "__main__":
    print("""This cipher is approved by Disk Jockey B.

1. Encrypt input
2. Encrypt flag
""")


    while True:
        inp = input("> ")

        match inp:
            case '1':
                print(encrypt(input("? ").encode()).hex())
            case '2':
                print(encrypt(flag).hex())
            case _:
                print("Bye!")
                exit()

```


```python

def encrypt(data):
    global state, buffer
    print("state before encryption:", state)
    print("buffer: ", len(buffer))
    output = []
    for b in data:
        if len(buffer) == 0:
            buffer = b"".join(long_to_bytes(x).rjust(4, b"\x00") for x in state)
            state = chacha_block(state)
            print("changed state:", state)
        output.append(b ^ buffer[0])
        buffer = buffer[1:]
    return bytes(output)

```

encryption is actually only data xor state. 

Important thing you should know about xor is if a ^ b = c then c ^ a = b and c ^ b = a, 

also order doesn't matter so a ^ b = b ^ a. It's very convenient when comes to exploiting simple xor encryption.

In our script if we change code and insert same state 
```python

def encrypt(data):
    global state, buffer
    print("state before encryption:", state)
    print("buffer: ", len(buffer))
    output = []
    for b in data:
        if len(buffer) == 0:
            buffer = b"".join(long_to_bytes(x).rjust(4, b"\x00") for x in state)
            state = chacha_block(state)
            print("changed state:", state)
        output.append(b ^ buffer[0])
        buffer = buffer[1:]
    return bytes(output)

```

When you realize this then you'll know you don't have to make decryption function. You can reverse operations with same function if you just knew state before 

You may ask now how to steal state? 

Well, it's xor again

If (data[i] ^ buffer[i] = encryption[i]) then (encryption[i] ^ data[i] = buffer[i]) 

note that buffer is same value as state but buffer is shown as characters and state is shown as numbers. 

# finalizing

Now we want to make sript that gathers buffer from encryption and our custom string.

mine looks like that

```python

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


```


state is 64 byte long so we need to insert 64 character string to steal state variable. you need to enter 64 characters for 2 reasons:
 - not smaller than 64 cause state has 64 bytes and you need to capture whole state,
 - also not bigger than 64 due length of buffer should be 0 at the beggining of encryption so script will just start with new clean chacha_block

It's best for input string to look like '0000000000000000000000000000000000000000000000000000000000000000' to keep things simple


make sure to run chacha_block on state to change it same way it will be changed by chal.py script
I just call encrypt function so it happend automaticaly

At last we insert flag from script and challange is done

```python
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
```
    

I lost my flag unfortunately but here is fabricated one
