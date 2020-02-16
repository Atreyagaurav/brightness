package main

import "fmt"
import "flag"
import "io/ioutil"
// import "os"

func main () {
	brightness_percentage := flag.Int("percentage",50,"brightness percentage")
	brightness_up := flag.Bool("up",false,"increase brightness")
	brightness_down := flag.Bool("down",false,"decrease brightness")
	brightness_dir := flag.String("directory","/sys/class/backlight/intel_backlight/","directory location of brightness files")
	flag.Parse()
	
	max_file := *brightness_dir+"max_brightness"
	main_file := *brightness_dir+"brightness"
	
	max_value := read_brightness(max_file)
	value :=read_brightness(main_file)
	percentage := calc_percentage(value,max_value)
	
	if *brightness_up == true{
		percentage += 10
	} else if *brightness_down == true{
		percentage -= 10
	}else {
		percentage = int(*brightness_percentage)	
	}
	parse_brightness(&percentage)
	value = calc_value(percentage, max_value)
	write_brightness(value,main_file)
}

func parse_err (err error) {
	if err!=nil {
		panic(err)
	}
}
func parse_brightness (b *int) {
	if *b > 100 {
		*b = 100
	}else if *b < 5 {
		*b = 5
	}
}

func calc_percentage (val int, max int) int {
	return val * 100 / max
}

func calc_value (perc int, max int) int {
	return perc * max / 100
}

func read_brightness (filePath string) int {
	var value int
	read,err := ioutil.ReadFile(filePath)
	parse_err(err)
	_,err= fmt.Sscanf(string(read),"%d",&value)
	parse_err(err)
	return value
}

func write_brightness (value int, filepath string) {
	value_string := fmt.Sprint(value)
	err := ioutil.WriteFile(filepath, []byte(string(value_string)), 0644)
	parse_err(err)
}
