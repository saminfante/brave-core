/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

export const addSiteCosmeticFilter = async (origin: string, cssfilter: string) => {
  chrome.storage.local.get('cosmeticFilterList', (storeData = {}) => {
    let storeList = Object.assign({}, storeData.cosmeticFilterList)
    if (storeList[origin] === undefined || storeList[origin].length === 0) { // nothing in filter list for origin
      storeList[origin] = [cssfilter]
    } else { // add entry
      storeList[origin].push(cssfilter)
    }
    chrome.storage.local.set({ 'cosmeticFilterList': storeList })
  })
}

export const removeSiteFilter = (origin: string) => {
  chrome.storage.local.get('cosmeticFilterList', (storeData = {}) => {
    let storeList = Object.assign({}, storeData.cosmeticFilterList)
    delete storeList[origin]
    chrome.storage.local.set({ 'cosmeticFilterList': storeList })
  })
}

export const applyAdblockCosmeticFilters = (tabId: number, hostname: string) => {
  chrome.braveShields.hostnameCosmeticResources(hostname, async (resources) => {
    const stylesheet = generateCosmeticBlockingStylesheet(resources.hide_selectors, resources.style_selectors)
    if (stylesheet) {
      chrome.tabs.insertCSS(tabId, {
        code: stylesheet,
        cssOrigin: 'user',
        runAt: 'document_start'
      })
    }

    chrome.tabs.sendMessage(tabId, {
      type: 'cosmeticFilterGenericExceptions',
      exceptions: resources.exceptions
    })

    if (resources.injected_script) {
      chrome.tabs.executeScript(tabId, {
        code: resources.injected_script,
        runAt: 'document_start'
      })
    }
  })
}

export const applyCSSCosmeticFilters = (tabId: number, hostname: string) => {
  chrome.storage.local.get('cosmeticFilterList', (storeData = {}) => {
    if (!storeData.cosmeticFilterList) {
      if (process.env.NODE_ENV === 'shields_development') {
        console.log('applySiteFilters: no cosmetic filter store yet')
      }
      return
    }
    if (storeData.cosmeticFilterList[hostname] !== undefined) {
      storeData.cosmeticFilterList[hostname].map((rule: string) => {
        if (process.env.NODE_ENV === 'shields_development') {
          console.log('applying rule', rule)
        }
        chrome.tabs.insertCSS(tabId, { // https://github.com/brave/brave-browser/wiki/Cosmetic-Filtering
          code: `${rule} {display: none !important;}`,
          cssOrigin: 'user',
          runAt: 'document_start'
        })
      })
    }
  })
}

export const removeAllFilters = () => {
  chrome.storage.local.set({ 'cosmeticFilterList': {} })
}

const generateCosmeticBlockingStylesheet = (hide_selectors: string[], style_selectors: any) => {
  let stylesheet = ""
  if (hide_selectors.length > 0) {
    stylesheet += hide_selectors[0]
    for(const selector of hide_selectors.slice(1)) {
      stylesheet += ',' + selector
    }
    stylesheet += '{display:none !important;}\n'
  }
  for (const selector in style_selectors) {
    stylesheet += selector + '{' + style_selectors[selector] + '\n'
  }

  return stylesheet
}
