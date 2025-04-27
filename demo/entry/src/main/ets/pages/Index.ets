import { add, area, square, str_add } from 'libentry.so';
import { promptAction } from '@kit.ArkUI';
import json from '@ohos.util.json';

@Entry
@Component
struct Index {
  @State message: string = 'Hello World';

  build() {
    Row() {
      Column({
        space: 12
      }) {
        Button('Add').onClick((evt) => {
          promptAction.showToast({
            message: `${add(1, 2)}`
          })
        })
        Button('Area').onClick((evt) => {
          promptAction.showToast({
            message: `${area({
              width: 10, height: 2
            })}`
          })
        })
        Button('Square').onClick((evt) => {
          promptAction.showToast({
            message: `${json.stringify(square(10))}`
          })
        })
        Button('StrAdd').onClick((evt) => {
          promptAction.showToast({
            message: `${str_add('area: ', area({ width: 2, height: 3 }))}`
          })
        })
      }
      .width('100%')
    }
    .height('100%')
  }
}
