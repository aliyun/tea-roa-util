package service

import (
	"reflect"
	"testing"

	"github.com/alibabacloud-go/tea/tea"
	"github.com/alibabacloud-go/tea/utils"
)

func Test_GetSignature(t *testing.T) {
	request := tea.NewRequest()
	sign := GetStringToSign(request)
	signature := GetSignature(sign, tea.String("secret"))
	utils.AssertEqual(t, 28, len(tea.StringValue(signature)))
}

func Test_Sorter(t *testing.T) {
	tmp := map[string]string{
		"key":   "ccp",
		"value": "ok",
	}
	sort := newSorter(tmp)
	sort.Sort()

	len := sort.Len()
	utils.AssertEqual(t, len, 2)

	isLess := sort.Less(0, 1)
	utils.AssertEqual(t, isLess, true)

	sort.Swap(0, 1)
	isLess = sort.Less(0, 1)
	utils.AssertEqual(t, isLess, false)
}

func Test_getStringToSign(t *testing.T) {
	request := tea.NewRequest()
	request.Query = map[string]*string{
		"ccp": tea.String("ok"),
	}
	request.Headers = map[string]*string{
		"x-acs-meta": tea.String("user"),
	}
	str := getStringToSign(request)
	utils.AssertEqual(t, 28, len(str))
}

func Test_ToForm(t *testing.T) {
	filter := map[string]interface{}{
		"client": "test",
		"tag": map[string]*string{
			"key": tea.String("value"),
		},
		"strs": []string{"str1", "str2"},
	}

	result := ToForm(filter)
	utils.AssertEqual(t, "client=test&strs.1=str1&strs.2=str2&tag.key=value", tea.StringValue(result))
}

func Test_flatRepeatedList(t *testing.T) {
	filter := map[string]interface{}{
		"client":  "test",
		"version": "1",
		"null":    nil,
		"slice": []interface{}{
			map[string]interface{}{
				"map": "valid",
			},
			6,
		},
		"map": map[string]interface{}{
			"value": "ok",
		},
	}

	result := make(map[string]*string)
	for key, value := range filter {
		filterValue := reflect.ValueOf(value)
		flatRepeatedList(filterValue, result, key)
	}
	utils.AssertEqual(t, tea.StringValue(result["slice.1.map"]), "valid")
	utils.AssertEqual(t, tea.StringValue(result["slice.2"]), "6")
	utils.AssertEqual(t, tea.StringValue(result["map.value"]), "ok")
	utils.AssertEqual(t, tea.StringValue(result["client"]), "test")
	utils.AssertEqual(t, tea.StringValue(result["slice.1.map"]), "valid")
}
