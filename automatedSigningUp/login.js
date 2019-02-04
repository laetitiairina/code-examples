#!/usr/bin/env node
const puppeteer = require('puppeteer')
const fs = require('fs')

async function run() {

  // const uzh = "#userIdPSelection_iddlist > div:nth-child(14)"
  // const eth = "#userIdPSelection_iddlist > div:nth-child(5)"
  const information = [["#userIdPSelection_iddlist > div:nth-child(14)", "./creds_Laetitia"], ["#userIdPSelection_iddlist > div:nth-child(5)", "./creds_Chli"]]

  // get lessonNumber, has to go up by one every week.
  const lessonNumber = parseInt(fs.readFileSync("number.txt"))
  fs.writeFileSync('number.txt', (lessonNumber + 1).toString())

  // do it twice. Once for chli and once for me
  for (let [uni, creds] of information) {

    const browser = await puppeteer.launch({
      headless: true
    })

    // tries to access page
    const page = await browser.newPage()
    await Promise.all([
      page.waitForNavigation({waitUntil: ['networkidle2', 'load', 'domcontentloaded']}),
      page.goto('a sports lessons page' + lessonNumber)
    ])

    // select login
    await page.waitForSelector('body > app-root > div > div:nth-child(2) > app-lesson-details > div > div > app-lessons-enrollment-button > alert > div')
    await page.click('body > app-root > div > div:nth-child(2) > app-lesson-details > div > div > app-lessons-enrollment-button > button')

    // select SwitchAAI login
    await page.waitForSelector('#switch-aai > div.panel-heading > strong')
    await page.click('#switch-aai > div.panel-body > div > form > div > p > button')

    // select the right university
    await page.waitForSelector('#userIdPSelection_iddtext')
    await page.evaluate((uni) => {
      const select = document.querySelector(uni).click()
      document.querySelector('#userInputArea > div:nth-child(2) > input[type="submit"]').click()
    }, uni)
    await page.waitForNavigation({ waitUntil: ['networkidle2', 'load', 'domcontentloaded']})

    // AAI sign in page
    await Promise.all([
      page.waitForNavigation({ waitUntil: ['networkidle2', 'load', 'domcontentloaded']}),
      page.evaluate ((CREDS) => {
        document.querySelector('#username').value = CREDS.username
        document.querySelector('#password').value = CREDS.password
        if (CREDS.username === "lbrits") {
          document.querySelector('body > div > div.aai_login_field > form > div:nth-child(4) > button').click()
        }
        else {
          document.querySelector('#LoginButtonText').click()
        }
      }, require(creds))
    ])

    // signing in for lesson
    await page.waitForSelector('#btnRegister')
    while(await tryAgain(page)) {
      await page.click('#btnRegister')
    }
    await browser.close()
  }
}

async function tryAgain (page) {
  return await page.evaluate ((nothing) => {
    return (document.querySelector('#btnRegister').className === "btn-default btn disabled enrollmentPlacePadding" && document.querySelector("#btnRegister").innerHTML.includes("FÜR LEKTION EINSCHREIBEN"))
  })
}

run()