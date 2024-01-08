# What's a rune - irisctf 2024

```ruby

package main

import (
	"fmt"
	"os"
	"strings"
)

var flag = "irisctf{this_is_not_the_real_flag}"

func init() {
	runed := []string{}
	z := rune(0)

	for _, v := range flag {
		runed = append(runed, string(v+z))
		z = v
	}

	flag = strings.Join(runed, "")
}

func main() {
	
	fmt.Println(flag);
	fmt.Println(string(rune(0)));
	
	file, err := os.OpenFile("the", os.O_RDWR|os.O_CREATE, 0644)
	if err != nil {
		fmt.Println(err)
		return
	}

	defer file.Close()
	if _, err := file.Write([]byte(flag)); err != nil {
		fmt.Println(err)
		return
	}
}


```

rune() is function that gets keycode of character ,string() gets string from keycode

if we change variable names script becomes clear

```ruby


func init() {
	runed := []string{}
	prev_char := rune(0)

	for _, curr_char := range flag {
		runed = append(runed, string(curr_char + prev_char))
		prev_char = curr_char
	}

	flag = strings.Join(runed, "")
}
```

# solution

to decode we change runed = append(runed, string(curr_char + prev_char)) prev_char = curr_char

to runed = append(runed, string(curr_char - prev_char)); prev_char = curr_char - prev_char


```ruby

package main

import (
	"fmt"
	"strings"
)

var flag = "iÛÛÜÖ×ÚáäÈÑ¥gebªØÔÍãâ£i¥§²ËÅÒÍÈä"

func init() {
	runed := []string{}
	z := rune(0)

	for _, v := range flag {
		z = v - z
		runed = append(runed, string(z))
		
	}

	flag = strings.Join(runed, "")
}

func main() {
	
	fmt.Println(flag);
}


```

