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
